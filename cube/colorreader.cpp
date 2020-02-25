#include "colorreader.h"

ColorReader::ColorReader(std::map<std::string, Square> squares, std::map<std::string, double> configValues
, std::shared_ptr<Microcontroller> microcontroller)
: squares{squares} , configValues{configValues}, microcontroller{microcontroller} {}

void ColorReader::read()
{
    cv::VideoCapture webcam(WEBCAM_ID);
    std::vector<Face> faces = {Face::UP, Face::FRONT, Face::DOWN, Face::BACK, Face::RIGHT, Face::LEFT};
    std::map<std::string, double> faceHues;
    cv::Mat picture;
    std::vector<std::vector<cv::Point>> squaresIdentified;
    std::vector<Color> identifiedColors;
    for(int faceIndex = 0; faceIndex < faces.size(); faceIndex++)
    {
        // Sometimes the camera takes time to initialize
        while (!webcam.isOpened()) { webcam.open(WEBCAM_ID); }
        // 1 second for the webcam lens to get used to the brightness
        Utils::delay(1000); 

        while(squaresIdentified.size() != SQUARES_PER_FACE)
        {
            squaresIdentified.clear();
            findSquares(picture, squaresIdentified);
            webcam >> picture;
            cv::flip(picture,picture,-1);
        }
        faceHues = getFaceHues(picture, squaresIdentified);
        identifiedColors = identifyColors(faceHues, configValues);
        drawSquares(picture, squaresIdentified, identifiedColors);
        updateModel(faces[faceIndex], identifiedColors);
        microcontroller->runCommand(Utils::int2command(faceIndex+1));
    }
    webcam.~VideoCapture();
}

void ColorReader::updateModel(Face currentFace, std::vector<Color> identifiedColors)
{
    std::string faceStr =  Utils::face2str(currentFace);
    for(size_t squareIndex = 1 ; squareIndex <= SQUARES_PER_FACE; squareIndex++)
        squares[faceStr + std::to_string(squareIndex)].setColor(identifiedColors[squareIndex-1]);
}
