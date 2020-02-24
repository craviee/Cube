#include "colorcalibrator.h"

ColorCalibrator::ColorCalibrator(std::map<std::string, Square> squares) : squares{squares}
{
    configExists() ? readConfig() : initializeConfig();
}

bool ColorCalibrator::configExists()
{
    struct stat buffer;   
    return (stat(std::string(CONFIG_FILE).c_str(), &buffer) == 0); 
}

void ColorCalibrator::readConfig()
{
    std::fstream configFile;
    std::string line;
    configFile.open(CONFIG_FILE, std::fstream::in | std::fstream::out);
    getline(configFile,line); configValues["GreenFaceRedHue"] = std::stod(line);
    getline(configFile,line); configValues["GreenFaceGreenHue"] = std::stod(line);
    getline(configFile,line); configValues["GreenFaceBlueHue"] = std::stod(line);
    getline(configFile,line); configValues["RedFaceRedHue"] = std::stod(line);
    getline(configFile,line); configValues["RedFaceGreenHue"] = std::stod(line);
    getline(configFile,line); configValues["RedFaceBlueHue"] = std::stod(line);
    getline(configFile,line); configValues["BlueFaceRedHue"] = std::stod(line);
    getline(configFile,line); configValues["BlueFaceGreenHue"] = std::stod(line);
    getline(configFile,line); configValues["BlueFaceBlueHue"] = std::stod(line);
    getline(configFile,line); configValues["OrangeFaceRedHue"] = std::stod(line);
    getline(configFile,line); configValues["OrangeFaceGreenHue"] = std::stod(line);
    getline(configFile,line); configValues["OrangeFaceBlueHue"] = std::stod(line);
    getline(configFile,line); configValues["YellowFaceRedHue"] = std::stod(line);
    getline(configFile,line); configValues["YellowFaceGreenHue"] = std::stod(line);
    getline(configFile,line); configValues["YellowFaceBlueHue"] = std::stod(line);
    getline(configFile,line); configValues["WhiteFaceRedHue"] = std::stod(line);
    getline(configFile,line); configValues["WhiteFaceGreenHue"] = std::stod(line);
    getline(configFile,line); configValues["WhiteFaceBlueHue"] = std::stod(line);
    configFile.close();
}

void ColorCalibrator::writeConfig()
{
    std::fstream configFile;
    configFile.open(CONFIG_FILE, std::fstream::in | std::fstream::out);
    configFile << configValues["GreenFaceRedHue"] << std::endl;
    configFile << configValues["GreenFaceGreenHue"] << std::endl;
    configFile << configValues["GreenFaceBlueHue"] << std::endl;
    configFile << configValues["RedFaceRedHue"] << std::endl;
    configFile << configValues["RedFaceGreenHue"] << std::endl;
    configFile << configValues["RedFaceBlueHue"] << std::endl;
    configFile << configValues["BlueFaceRedHue"] << std::endl;
    configFile << configValues["BlueFaceGreenHue"] << std::endl;
    configFile << configValues["BlueFaceBlueHue"] << std::endl;
    configFile << configValues["OrangeFaceRedHue"] << std::endl;
    configFile << configValues["OrangeFaceGreenHue"] << std::endl;
    configFile << configValues["OrangeFaceBlueHue"] << std::endl;
    configFile << configValues["YellowFaceRedHue"] << std::endl;
    configFile << configValues["YellowFaceGreenHue"] << std::endl;
    configFile << configValues["YellowFaceBlueHue"] << std::endl;
    configFile << configValues["WhiteFaceRedHue"] << std::endl;
    configFile << configValues["WhiteFaceGreenHue"] << std::endl;
    configFile << configValues["WhiteFaceBlueHue"] << std::endl;
    configFile.close();
}

void ColorCalibrator::initializeConfigFile()
{
    std::ofstream initialConfigFile;
    initialConfigFile.open(CONFIG_FILE);
    for(int i = 0; i < CALIBRATION_HUE_VALUES_QUANTITY; i++)
        initialConfigFile << "0" << std::endl;
    initialConfigFile.close();
}

void ColorCalibrator::initializeConfig()
{
    configValues["GreenFaceRedHue"] = 0;
    configValues["GreenFaceGreenHue"] = 0;
    configValues["GreenFaceBlueHue"] = 0;
    configValues["RedFaceRedHue"] = 0;
    configValues["RedFaceGreenHue"] = 0;
    configValues["RedFaceBlueHue"] = 0;
    configValues["BlueFaceRedHue"] = 0;
    configValues["BlueFaceGreenHue"] = 0;
    configValues["BlueFaceBlueHue"] = 0;
    configValues["OrangeFaceRedHue"] = 0;
    configValues["OrangeFaceGreenHue"] = 0;
    configValues["OrangeFaceBlueHue"] = 0;
    configValues["YellowFaceRedHue"] = 0;
    configValues["YellowFaceGreenHue"] = 0;
    configValues["YellowFaceBlueHue"] = 0;
    configValues["WhiteFaceRedHue"] = 0;
    configValues["WhiteFaceGreenHue"] = 0;
    configValues["WhiteFaceBlueHue"] = 0;
}

void ColorCalibrator::calibrate()
{
    std::vector<Face> faces = {Face::UP, Face::FRONT, Face::DOWN, Face::BACK, Face::RIGHT, Face::LEFT};
    Arduino robot;
    const char *command;

    initializeConfig();
    for(int faceIndex = 0; faceIndex < faces.size(); faceIndex++)
    {
        calibrateFace(faces[faceIndex]);
        command = std::string("0" + std::to_string(faceIndex+1)).c_str();
        robot.runCommand(command);
    }
    // Divide each Hue by the time it was identified to get its average
    for (std::map<std::string,double>::iterator it=configValues.begin(); it!=configValues.end(); ++it)
        it->second /= CALIBRATION_PICTURES_PER_FACE * SQUARES_PER_FACE;
    writeConfig();
}

void ColorCalibrator::calibrateFace(Face currentFace)
{
    cv::VideoCapture webcam(WEBCAM_ID);
    cv::Mat picture;
    std::map<std::string, double> faceHues;
    std::vector<std::vector<cv::Point>> squaresReaded;
    for(int i = 0; i < CALIBRATION_PICTURES_PER_FACE; i++)
    {
        // Sometimes the camera takes time to initialize
        while (!webcam.isOpened()) { webcam.open(WEBCAM_ID); }
        // 1 second for the webcam lens to get used to the brightness
        Utils::delay(1000);

        webcam >> picture;

        // The flip is needed here because the camera is flipped 180º in my setup
        cv::flip(picture,picture,-1);

        while(squaresReaded.size() != SQUARES_PER_FACE)
        {
            squares.clear();
            findSquares(picture, squaresReaded);
            webcam >> picture;
            cv::flip(picture,picture,-1);
        }
        
        faceHues = getFaceHues(picture, squaresReaded);

        for(int squareIndex = 1 ; squareIndex <= SQUARES_PER_FACE; squareIndex++)
        {
            sumCalibrationHues(squareIndex, currentFace, faceHues);
        }
    }
    webcam.~VideoCapture();
}

void ColorCalibrator::sumCalibrationHues(int squareIndex, Face currentFace, std::map<std::string, double> faceHues)
{
    std::string faceStr = Utils::face2str(currentFace);
    Color currentColor = squares[faceStr + std::to_string(squareIndex)].getColor();
    std::string colorStr = Utils::color2str(currentColor);

    configValues[colorStr + "FaceRedHue"] += faceHues[std::to_string(squareIndex) + "RedHue"];
    configValues[colorStr + "FaceGreenHue"] += faceHues[std::to_string(squareIndex) + "GreenHue"];
    configValues[colorStr + "FaceBlueHue"] += faceHues[std::to_string(squareIndex) + "BlueHue"];
}
