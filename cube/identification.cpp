#pragma once

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <string>
#include <map>
#include <math.h>
#include <string.h>
#include "constants.h"
#include "enums.h"

static std::vector<std::vector<cv::Point>> orderIdentifications(std::vector<std::vector<cv::Point>> squares)
{
    int min_x_first, min_x_second, min_y_first, min_y_second;

    for(int first = 0; first < squares.size()-1; first++)
    {
        min_x_first = std::min(squares[first][0].x, std::min(squares[first][1].x , std::min(squares[first][2].x, squares[first][3].x)));
        min_y_first = std::min(squares[first][0].y, std::min(squares[first][1].y , std::min(squares[first][2].y, squares[first][3].y)));

        for(int second = first+1; second < squares.size(); second++)
        {
            min_x_second = std::min(squares[second][0].x, std::min(squares[second][1].x , std::min(squares[second][2].x, squares[second][3].x)));
            min_y_second = std::min(squares[second][0].y, std::min(squares[second][1].y , std::min(squares[second][2].y, squares[second][3].y)));
            if((min_y_second*2 + min_x_second) < (min_y_first*2 + min_x_first))
            {
                std::vector<cv::Point> aux = squares[second];
                squares[second] = squares[first];
                squares[first] = aux;
            }
        }
    }
    return squares;
}
static cv::Scalar getScalar(Color color)
{
    if(color == Color::GREEN) return cv::Scalar(0,255,0);
    else if(color == Color::BLUE) return cv::Scalar(255,0,0);
    else if(color == Color::RED) return cv::Scalar(0,0,255);
    else if(color == Color::WHITE) return cv::Scalar(255,255,255);
    else if(color == Color::YELLOW) return cv::Scalar(0,255,255);
    else if(color == Color::ORANGE) return cv::Scalar(0,165,255);
    else return cv::Scalar(0,0,0);
}
static Color detectColor(double redHue, double greenHue, double blueHue, std::map<std::string, double> configValues)
{
    Color identifiedColor = Color::ERROR;
    // Starting with the largest distance possible
    double colorDistance = 255*255*255;

    double greenProximity = std::fabs(redHue-configValues["GreenFaceRedHue"]) 
    + std::fabs(greenHue-configValues["GreenFaceGreenHue"]) + std::fabs(blueHue-configValues["GreenFaceBlueHue"]);
    double redProximity = std::fabs(redHue-configValues["RedFaceRedHue"]) 
    + std::fabs(greenHue-configValues["RedFaceGreenHue"]) + std::fabs(blueHue-configValues["RedFaceBlueHue"]);
    double blueProximity = std::fabs(redHue-configValues["BlueFaceRedHue"]) 
    + std::fabs(greenHue-configValues["BlueFaceGreenHue"]) + std::fabs(blueHue-configValues["BlueFaceBlueHue"]);
    double orangeProximity = std::fabs(redHue-configValues["OrangeFaceRedHue"]) 
    + std::fabs(greenHue-configValues["OrangeFaceGreenHue"]) + std::fabs(blueHue-configValues["OrangeFaceBlueHue"]);
    double yellowProximity = std::fabs(redHue-configValues["YellowFaceRedHue"]) 
    + std::fabs(greenHue-configValues["YellowFaceGreenHue"]) + std::fabs(blueHue-configValues["YellowFaceBlueHue"]);
    double whiteProximity = std::fabs(redHue-configValues["WhiteFaceRedHue"]) 
    + std::fabs(greenHue-configValues["WhiteFaceGreenHue"]) + std::fabs(blueHue-configValues["WhiteFaceBlueHue"]);

    if(colorDistance > greenProximity) { colorDistance = greenProximity; identifiedColor = Color::GREEN; }
    if(colorDistance > redProximity) { colorDistance = redProximity; identifiedColor = Color::RED; }
    if(colorDistance > blueProximity) { colorDistance = blueProximity; identifiedColor = Color::BLUE; }
    if(colorDistance > orangeProximity) { colorDistance = orangeProximity; identifiedColor = Color::ORANGE; }
    if(colorDistance > yellowProximity) { colorDistance = yellowProximity; identifiedColor = Color::YELLOW; }
    if(colorDistance > whiteProximity) { colorDistance = whiteProximity; identifiedColor = Color::WHITE; }

    return identifiedColor;
}

static double angle( cv::Point pt1, cv::Point pt2, cv::Point pt0 )
{
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}
static void findSquares( const cv::Mat& image, std::vector<std::vector<cv::Point> >& squares )
{
    cv::Mat timg(image);
    medianBlur(image, timg, 9);
    cv::Mat gray0(timg.size(), CV_8U), gray;
    bool isRepeated;
    int average_x, average_y, max_x,min_x,max_y,min_y,
    current_max_x,current_min_x,current_max_y,current_min_y;
    std::vector<std::vector<cv::Point> > contours;
    for( int c = 0; c < 3; c++ )
    {
        int ch[] = {c, 0};
        mixChannels(&timg, 1, &gray0, 1, ch, 1);
        for( int l = 0; l < 10; l++ )
        {
            if( l == 0 )
            {
                Canny(gray0, gray, 5, 10, 5);
                dilate(gray, gray, cv::Mat(), cv::Point(-1,-1));
            }
            else
            {
                gray = gray0 >= (l+1)*255/10;
            }
            findContours(gray, contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);
            std::vector<cv::Point> approx;
            for( size_t i = 0; i < contours.size(); i++ )
            {
                approxPolyDP(cv::Mat(contours[i]), approx, arcLength(cv::Mat(contours[i]), true)*0.02, true);
                if( approx.size() == 4 &&
                    fabs(contourArea(cv::Mat(approx))) > 1000 &&
                    isContourConvex(cv::Mat(approx)) )
                {
                    double maxCosine = 0;
                    isRepeated = false;
                    for( int j = 2; j < 5; j++ )
                    {
                        double cosine = fabs(angle(approx[j%4], approx[j-2], approx[j-1]));
                        maxCosine = MAX(maxCosine, cosine);
                    }
                    if( maxCosine < 0.3 )
                    {
                        current_max_x = std::max(approx[0].x, std::max(approx[1].x , std::max(approx[2].x, approx[3].x)));
                        current_max_y = std::max(approx[0].y, std::max(approx[1].y , std::max(approx[2].y, approx[3].y)));
                        current_min_x = std::min(approx[0].x, std::min(approx[1].x , std::min(approx[2].x, approx[3].x)));
                        current_min_y = std::min(approx[0].y, std::min(approx[1].y , std::min(approx[2].y, approx[3].y)));
                        average_x = (current_max_x + current_min_x)/2;
                        average_y = (current_max_y + current_min_y)/2;
                        for(int i = 0; i < squares.size(); i++)
                        {
                            max_x = std::max(squares[i][0].x, std::max(squares[i][1].x , std::max(squares[i][2].x, squares[i][3].x)));
                            max_y = std::max(squares[i][0].y, std::max(squares[i][1].y , std::max(squares[i][2].y, squares[i][3].y)));
                            min_x = std::min(squares[i][0].x, std::min(squares[i][1].x , std::min(squares[i][2].x, squares[i][3].x)));
                            min_y = std::min(squares[i][0].y, std::min(squares[i][1].y , std::min(squares[i][2].y, squares[i][3].y)));
                            if(average_x > min_x && average_x < max_x &&
                                average_y > min_y && average_y < max_y)
                            {
                                isRepeated = true;
                                break;
                            }
                        }
                        if(!isRepeated)
                            squares.push_back(approx);
                    }
                }
            }
        }
    }
}

static void drawSquares( cv::Mat& image, std::vector<std::vector<cv::Point>>& squares, std::vector<Color> colors)
{
    for( size_t i = 0; i < squares.size(); i++ )
    {
        const cv::Point* p = &squares[i][0];
        int n = (int)squares[i].size();
        polylines(image, &p, &n, 1, true, getScalar(colors[i]), 3, cv::LINE_AA);
    }
}

static cv::Scalar getHuesFromSquare(cv::Mat& image, std::vector<cv::Point>& square)
{
    cv::Point pts[1][4];
    pts[0][0] = cv::Point(square[0].x, square[0].y);
    pts[0][1] = cv::Point(square[1].x, square[1].y);
    pts[0][2] = cv::Point(square[2].x, square[2].y);
    pts[0][3] = cv::Point(square[3].x, square[3].y);
    double max_x = std::max(square[0].x, std::max(square[1].x , std::max(square[2].x, square[3].x)));
    double max_y = std::max(square[0].y, std::max(square[1].y , std::max(square[2].y, square[3].y)));
    double min_x = std::min(square[0].x, std::min(square[1].x , std::min(square[2].x, square[3].x)));
    double min_y = std::min(square[0].y, std::min(square[1].y , std::min(square[2].y, square[3].y)));
    cv::Mat miniMat = cv::Mat(image, cv::Rect(min_x, min_y, max_x-min_x, max_y-min_y));
    return cv::mean(miniMat);
}

static std::map<std::string, double> getFaceHues(cv::Mat& image, std::vector<std::vector<cv::Point>>& squaresReaded)
{
    std::map<std::string, double> faceHues;
    squaresReaded = orderIdentifications(squaresReaded);
    cv::Scalar identifiedColor;
    double redHue,greenHue,blueHue;
    for( size_t i = 0; i < squaresReaded.size(); i++ )
    {
        identifiedColor = getHuesFromSquare(image, squaresReaded[i]);
        redHue = identifiedColor[0];
        greenHue = identifiedColor[1];
        blueHue = identifiedColor[2];
        faceHues[std::to_string(i+1) + "RedHue"] = redHue;
        faceHues[std::to_string(i+1) + "GreenHue"] = greenHue;
        faceHues[std::to_string(i+1) + "BlueHue"] = blueHue;
    }
    return faceHues;
}

static std::vector<Color> identifyColors(std::map<std::string, double> faceHues, std::map<std::string, double> configValues)
{
    std::vector<Color> identifiedColors;
    double redHue, greenHue, blueHue;
    for( size_t i = 0; i < faceHues.size(); i++ )
    {
        redHue = faceHues[std::to_string(i+1) + "RedHue"];
        greenHue = faceHues[std::to_string(i+1) + "GreenHue"];
        blueHue = faceHues[std::to_string(i+1) + "BlueHue"];
        identifiedColors.push_back(detectColor(redHue, greenHue, blueHue, configValues));
    }
    return identifiedColors;
}
