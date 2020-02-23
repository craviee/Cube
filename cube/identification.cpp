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

static std::string Char2StyleSheet(char c)
{
    std::string result = "background-color: ";
    if(c == 'B') return (result + "blue");
    else if(c == 'W') return (result + "white");
    else if(c == 'O') return (result + "orange");
    else if(c == 'R') return (result + "red");
    else if(c == 'G') return (result + "green");
    else if(c == 'Y') return (result + "yellow");
    else return (result + "black");
}
static char StyleSheet2Char(std::string c)
{
    if(c == "background-color: blue") return 'B';
    if(c == "background-color: white") return 'W';
    if(c == "background-color: orange") return 'O';
    if(c == "background-color: red") return 'R';
    if(c == "background-color: green") return 'G';
    if(c == "background-color: yellow") return 'Y';
    else return 'X';
}
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
static cv::Scalar colorForLine(char c)
{
    if(c == 'G')
        return cv::Scalar(0,255,0);
    else if(c == 'B')
        return cv::Scalar(255,0,0);
    else if(c == 'R')
        return cv::Scalar(0,0,255);
    else if(c == 'W')
        return cv::Scalar(255,255,255);
    else if(c == 'Y')
        return cv::Scalar(0,255,255);
    else if(c == 'O')
        return cv::Scalar(0,165,255);
    else
        return cv::Scalar(0,0,0);
}
static char detectColor(double r, double g, double b/*, config& color*/)
{
    char c = ' ';
    // double mais_proximo = 1000;
    // if(mais_proximo > (std::fabs(r-color.Gr) + std::fabs(g-color.Gg) + std::fabs(b-color.Gb)))
    // {
    //     mais_proximo = (std::fabs(r-color.Gr) + std::fabs(g-color.Gg) + std::fabs(b-color.Gb));
    //     c = 'G';
    // }
    // if(mais_proximo > (std::fabs(r-color.Rr) + std::fabs(g-color.Rg) + std::fabs(b-color.Rb)))
    // {
    //     mais_proximo = (std::fabs(r-color.Rr) + std::fabs(g-color.Rg) + std::fabs(b-color.Rb));
    //     c = 'R';
    // }
    // if(mais_proximo > (std::fabs(r-color.Br) + std::fabs(g-color.Bg) + std::fabs(b-color.Bb)))
    // {
    //     mais_proximo = (std::fabs(r-color.Br) + std::fabs(g-color.Bg) + std::fabs(b-color.Bb));
    //     c = 'B';
    // }
    // if(mais_proximo > (std::fabs(r-color.Or) + std::fabs(g-color.Og) + std::fabs(b-color.Ob)))
    // {
    //     mais_proximo = (std::fabs(r-color.Or) + std::fabs(g-color.Og) + std::fabs(b-color.Ob));
    //     c = 'O';
    // }
    // if(mais_proximo > (std::fabs(r-color.Yr) + std::fabs(g-color.Yg) + std::fabs(b-color.Yb)))
    // {
    //     mais_proximo = (std::fabs(r-color.Yr) + std::fabs(g-color.Yg) + std::fabs(b-color.Yb));
    //     c = 'Y';
    // }
    // if(mais_proximo > (std::fabs(r-color.Wr) + std::fabs(g-color.Wg) + std::fabs(b-color.Wb)))
    // {
    //     mais_proximo = (std::fabs(r-color.Wr) + std::fabs(g-color.Wg) + std::fabs(b-color.Wb));
    //     c = 'W';
    // }
    return c;
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

static std::map<std::string, double> drawSquares( cv::Mat& image, std::vector<std::vector<cv::Point> >& squares, std::vector<std::string>& colors/*, config& conf*/)
{
    std::map<std::string, double> result;
    squares = orderIdentifications(squares);
    int npoints = 4;
    cv::Point pts[1][4];
    cv::Scalar average;
    double r=0,g=0,b=0,max_x,min_x,max_y,min_y;
    char c;
    for( size_t i = 0; i < squares.size(); i++ )
    {
        const cv::Point* p = &squares[i][0];
        int n = (int)squares[i].size();
        if (p-> x > 3 && p->y > 3)
        {
            pts[0][0] = cv::Point(squares[i][0].x, squares[i][0].y);
            pts[0][1] = cv::Point(squares[i][1].x, squares[i][1].y);
            pts[0][2] = cv::Point(squares[i][2].x, squares[i][2].y);
            pts[0][3] = cv::Point(squares[i][3].x, squares[i][3].y);
            const cv::Point* points[1] = {pts[0]};
            max_x = std::max(squares[i][0].x, std::max(squares[i][1].x , std::max(squares[i][2].x, squares[i][3].x)));
            max_y = std::max(squares[i][0].y, std::max(squares[i][1].y , std::max(squares[i][2].y, squares[i][3].y)));
            min_x = std::min(squares[i][0].x, std::min(squares[i][1].x , std::min(squares[i][2].x, squares[i][3].x)));
            min_y = std::min(squares[i][0].y, std::min(squares[i][1].y , std::min(squares[i][2].y, squares[i][3].y)));
            cv::Mat miniMat = cv::Mat(image, cv::Rect(min_x, min_y, max_x-min_x, max_y-min_y));
            average = cv::mean(miniMat);
            r = average[0];
            g = average[1];
            b = average[2];
            if(squares.size() == SQUARES_PER_FACE)
            {
                result[std::to_string(i+1) + "RedHue"] = r;
                result[std::to_string(i+1) + "GreenHue"] = g;
                result[std::to_string(i+1) + "BlueHue"] = b;
            }
            //c = detectColor(r,g,b,conf);
            colors.push_back(Char2StyleSheet(c));
            polylines(image, &p, &n, 1, true, colorForLine(c), 3, cv::LINE_AA);
        }
    }
    return result;
}

static std::map<std::string, double> getFaceHues( cv::Mat& image, std::vector<std::vector<cv::Point>>& squaresReaded)
{
    std::map<std::string, double> faceHues;
    squaresReaded = orderIdentifications(squaresReaded);
    cv::Point pts[1][4];
    cv::Scalar average;
    cv::Mat miniMat;
    double redHue,greenHue,blueHue;
    double max_x,min_x,max_y,min_y;
    for( size_t i = 0; i < squaresReaded.size(); i++ )
    {
        const cv::Point* p = &squaresReaded[i][0];
        int n = (int)squaresReaded[i].size();
        if (p-> x > 3 && p->y > 3)
        {
            pts[0][0] = cv::Point(squaresReaded[i][0].x, squaresReaded[i][0].y);
            pts[0][1] = cv::Point(squaresReaded[i][1].x, squaresReaded[i][1].y);
            pts[0][2] = cv::Point(squaresReaded[i][2].x, squaresReaded[i][2].y);
            pts[0][3] = cv::Point(squaresReaded[i][3].x, squaresReaded[i][3].y);
            const cv::Point* points[1] = {pts[0]};
            max_x = std::max(squaresReaded[i][0].x, std::max(squaresReaded[i][1].x , std::max(squaresReaded[i][2].x, squaresReaded[i][3].x)));
            max_y = std::max(squaresReaded[i][0].y, std::max(squaresReaded[i][1].y , std::max(squaresReaded[i][2].y, squaresReaded[i][3].y)));
            min_x = std::min(squaresReaded[i][0].x, std::min(squaresReaded[i][1].x , std::min(squaresReaded[i][2].x, squaresReaded[i][3].x)));
            min_y = std::min(squaresReaded[i][0].y, std::min(squaresReaded[i][1].y , std::min(squaresReaded[i][2].y, squaresReaded[i][3].y)));
            miniMat = cv::Mat(image, cv::Rect(min_x, min_y, max_x-min_x, max_y-min_y));
            average = cv::mean(miniMat);
            redHue = average[0];
            greenHue = average[1];
            blueHue = average[2];
            if(squaresReaded.size() == SQUARES_PER_FACE)
            {
                faceHues[std::to_string(i+1) + "RedHue"] = redHue;
                faceHues[std::to_string(i+1) + "GreenHue"] = greenHue;
                faceHues[std::to_string(i+1) + "BlueHue"] = blueHue;
            }
        }
    }
    return faceHues;
}
