
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp":
#include <iostream>
#include <math.h>
#include <string.h>
#include "config.h"
namespace teste
{
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
    static std::vector<std::vector<cv::Point>> ordenaCubinho(std::vector<std::vector<cv::Point>> squares)
    {
        std::vector<cv::Point> aux;
        int menor_x_i, menor_x_j, menor_y_i, menor_y_j;
        for(int i = 0; i < squares.size(); i++)
        {
            menor_x_i = std::min(squares[i][0].x, std::min(squares[i][1].x , std::min(squares[i][2].x, squares[i][3].x)));
            menor_y_i = std::min(squares[i][0].y, std::min(squares[i][1].y , std::min(squares[i][2].y, squares[i][3].y)));
        }
        for(int i = 0; i < squares.size(); i++)
        {
            for(int j = i+1; j < squares.size(); j++)
            {
                menor_x_i = std::min(squares[i][0].x, std::min(squares[i][1].x , std::min(squares[i][2].x, squares[i][3].x)));
                menor_y_i = std::min(squares[i][0].y, std::min(squares[i][1].y , std::min(squares[i][2].y, squares[i][3].y)));
                menor_x_j = std::min(squares[j][0].x, std::min(squares[j][1].x , std::min(squares[j][2].x, squares[j][3].x)));
                menor_y_j = std::min(squares[j][0].y, std::min(squares[j][1].y , std::min(squares[j][2].y, squares[j][3].y)));
                if((menor_y_j*2 + menor_x_j) < (menor_y_i*2 + menor_x_i))
                {
                    aux = squares[j];
                    squares[j] = squares[i];
                    squares[i] = aux;
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
    static char detectColor(double r, double g, double b, config& color)
    {
        char c = ' ';
        double mais_proximo = 1000;
        if(mais_proximo > (std::fabs(r-color.Gr) + std::fabs(g-color.Gg) + std::fabs(b-color.Gb)))
        {
            mais_proximo = (std::fabs(r-color.Gr) + std::fabs(g-color.Gg) + std::fabs(b-color.Gb));
            c = 'G';
        }
        if(mais_proximo > (std::fabs(r-color.Rr) + std::fabs(g-color.Rg) + std::fabs(b-color.Rb)))
        {
            mais_proximo = (std::fabs(r-color.Rr) + std::fabs(g-color.Rg) + std::fabs(b-color.Rb));
            c = 'R';
        }
        if(mais_proximo > (std::fabs(r-color.Br) + std::fabs(g-color.Bg) + std::fabs(b-color.Bb)))
        {
            mais_proximo = (std::fabs(r-color.Br) + std::fabs(g-color.Bg) + std::fabs(b-color.Bb));
            c = 'B';
        }
        if(mais_proximo > (std::fabs(r-color.Or) + std::fabs(g-color.Og) + std::fabs(b-color.Ob)))
        {
            mais_proximo = (std::fabs(r-color.Or) + std::fabs(g-color.Og) + std::fabs(b-color.Ob));
            c = 'O';
        }
        if(mais_proximo > (std::fabs(r-color.Yr) + std::fabs(g-color.Yg) + std::fabs(b-color.Yb)))
        {
            mais_proximo = (std::fabs(r-color.Yr) + std::fabs(g-color.Yg) + std::fabs(b-color.Yb));
            c = 'Y';
        }
        if(mais_proximo > (std::fabs(r-color.Wr) + std::fabs(g-color.Wg) + std::fabs(b-color.Wb)))
        {
            mais_proximo = (std::fabs(r-color.Wr) + std::fabs(g-color.Wg) + std::fabs(b-color.Wb));
            c = 'W';
        }
        return c;
    }
}
using namespace cv;
using namespace std;
int thresh = 50, N = 10;
const char* wndname = "After Detection";
static double angle( Point pt1, Point pt2, Point pt0 )
{
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}
static void findSquares( const Mat& image, vector<vector<Point> >& squares )
{
    squares.clear();
    Mat timg(image);
    medianBlur(image, timg, 9);
    Mat gray0(timg.size(), CV_8U), gray;
    int x_atual_1, x_atual_2, y_atual_1, y_atual_2, repetido = 0, media_x, media_y, maior_x,menor_x,maior_y,menor_y, maior_x_atual,menor_x_atual,maior_y_atual,menor_y_atual;
    vector<vector<Point> > contours;
    for( int c = 0; c < 3; c++ )
    {
        int ch[] = {c, 0};
        mixChannels(&timg, 1, &gray0, 1, ch, 1);
        for( int l = 0; l < N; l++ )
        {
            if( l == 0 )
            {
                Canny(gray0, gray, 5, thresh, 5);
                dilate(gray, gray, Mat(), Point(-1,-1));
            }
            else
            {
                gray = gray0 >= (l+1)*255/N;
            }
            findContours(gray, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
            vector<Point> approx;
            for( size_t i = 0; i < contours.size(); i++ )
            {
                approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02, true);
                if( approx.size() == 4 &&
                    fabs(contourArea(Mat(approx))) > 1000 &&
                    isContourConvex(Mat(approx)) )
                {
                    double maxCosine = 0;
                    for( int j = 2; j < 5; j++ )
                    {
                        double cosine = fabs(angle(approx[j%4], approx[j-2], approx[j-1]));
                        maxCosine = MAX(maxCosine, cosine);
                    }
                    if( maxCosine < 0.3 )
                    {
                        maior_x_atual = std::max(approx[0].x, std::max(approx[1].x , std::max(approx[2].x, approx[3].x)));
                        maior_y_atual = std::max(approx[0].y, std::max(approx[1].y , std::max(approx[2].y, approx[3].y)));
                        menor_x_atual = std::min(approx[0].x, std::min(approx[1].x , std::min(approx[2].x, approx[3].x)));
                        menor_y_atual = std::min(approx[0].y, std::min(approx[1].y , std::min(approx[2].y, approx[3].y)));
                        media_x = (maior_x_atual + menor_x_atual)/2;
                        media_y = (maior_y_atual + menor_y_atual)/2;
                        for(int i = 0; i < squares.size(); i++)
                        {
                            maior_x = std::max(squares[i][0].x, std::max(squares[i][1].x , std::max(squares[i][2].x, squares[i][3].x)));
                            maior_y = std::max(squares[i][0].y, std::max(squares[i][1].y , std::max(squares[i][2].y, squares[i][3].y)));
                            menor_x = std::min(squares[i][0].x, std::min(squares[i][1].x , std::min(squares[i][2].x, squares[i][3].x)));
                            menor_y = std::min(squares[i][0].y, std::min(squares[i][1].y , std::min(squares[i][2].y, squares[i][3].y)));
                            if(media_x > menor_x && media_x < maior_x &&
                                media_y > menor_y && media_y < maior_y)
                            {
                                repetido = 1;
                                break;
                            }
                        }
                        if(repetido == 0)
                            squares.push_back(approx);
                        repetido = 0;
                    }
                }
            }
        }
    }
}
static std::vector<double> drawSquares( Mat& image, vector<vector<Point> >& squares, vector<std::string>& colors, config& conf)
{
    std::vector<double> result(squares.size()*3);
    squares = teste::ordenaCubinho(squares);
    int npoints = 4;
    Point pts[1][4];
    Scalar average;
    double r=0,g=0,b=0,maior_x,menor_x,maior_y,menor_y;
    char c;
    for( size_t i = 0; i < squares.size(); i++ )
    {
        const Point* p = &squares[i][0];
        int n = (int)squares[i].size();
        if (p-> x > 3 && p->y > 3)
        {
            pts[0][0] = Point(squares[i][0].x, squares[i][0].y);
            pts[0][1] = Point(squares[i][1].x, squares[i][1].y);
            pts[0][2] = Point(squares[i][2].x, squares[i][2].y);
            pts[0][3] = Point(squares[i][3].x, squares[i][3].y);
            const Point* points[1] = {pts[0]};
            maior_x = std::max(squares[i][0].x, std::max(squares[i][1].x , std::max(squares[i][2].x, squares[i][3].x)));
            maior_y = std::max(squares[i][0].y, std::max(squares[i][1].y , std::max(squares[i][2].y, squares[i][3].y)));
            menor_x = std::min(squares[i][0].x, std::min(squares[i][1].x , std::min(squares[i][2].x, squares[i][3].x)));
            menor_y = std::min(squares[i][0].y, std::min(squares[i][1].y , std::min(squares[i][2].y, squares[i][3].y)));
            cv::Mat miniMat = cv::Mat(image, cv::Rect(menor_x, menor_y, maior_x-menor_x, maior_y-menor_y));
            average = mean(miniMat);
            r = average[0];
            g = average[1];
            b = average[2];
            if(squares.size() == 9)
            {
                result[i*3] = r;
                result[i*3+1] = g;
                result[i*3+2] = b;
            }
            c = teste::detectColor(r,g,b,conf);
            colors.push_back(teste::Char2StyleSheet(c));
            polylines(image, &p, &n, 1, true, teste::colorForLine(c), 3, LINE_AA);
        }
    }
    imshow(wndname, image);
    return result;
}
