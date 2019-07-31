// The "Square Detector" program.
// It loads several images sequentially and tries to find squares in
// each image

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp":

#include <iostream>
#include <math.h>
#include <string.h>
#include "config.h"

//#define Gr 18.948
//#define Gg 40.4156
//#define Gb 17.7938

//#define Rr 9.60247
//#define Rg 14.93
//#define Rb 61.5331

//#define Br 41.8054
//#define Bg 17.9429
//#define Bb 16.1137

//#define Or 11.5653
//#define Og 31.9587
//#define Ob 96.7301

//#define Yr 20.8081
//#define Yg 80.2541
//#define Yb 101.48

//#define Wr 90.0695
//#define Wg 85.9425
//#define Wb 92.6204



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
//            std::cout << "não ordenado: " << menor_y_i*2 + menor_x_i << std::endl;
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
//            menor_x_i = std::min(squares[i][0].x, std::min(squares[i][1].x , std::min(squares[i][2].x, squares[i][3].x)));
//            menor_y_i = std::min(squares[i][0].y, std::min(squares[i][1].y , std::min(squares[i][2].y, squares[i][3].y)));
//            std::cout << "ordenado: " << menor_y_i*2 + menor_x_i << std::endl;
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
//        std::cout << "G " << (std::fabs(r-12.291) + std::fabs(g-53.92) + std::fabs(b-12.2859)) << std::endl;
        if(mais_proximo > (std::fabs(r-color.Gr) + std::fabs(g-color.Gg) + std::fabs(b-color.Gb)))
        {
            mais_proximo = (std::fabs(r-color.Gr) + std::fabs(g-color.Gg) + std::fabs(b-color.Gb));
            c = 'G';
        }

//        std::cout << "R " << (std::fabs(r-6.65937) + std::fabs(g-36.3531) + std::fabs(b-57.367)) << std::endl;
        if(mais_proximo > (std::fabs(r-color.Rr) + std::fabs(g-color.Rg) + std::fabs(b-color.Rb)))
        {
            mais_proximo = (std::fabs(r-color.Rr) + std::fabs(g-color.Rg) + std::fabs(b-color.Rb));
            c = 'R';
        }

//        std::cout << "B " << (std::fabs(r-35.7547) + std::fabs(g-28.4888) + std::fabs(b-1.37268)) << std::endl;
        if(mais_proximo > (std::fabs(r-color.Br) + std::fabs(g-color.Bg) + std::fabs(b-color.Bb)))
        {
            mais_proximo = (std::fabs(r-color.Br) + std::fabs(g-color.Bg) + std::fabs(b-color.Bb));
            c = 'B';
        }

//        std::cout << "O " << (std::fabs(r-6.57781) + std::fabs(g-52.2103) + std::fabs(b-94.1198)) << std::endl;
        if(mais_proximo > (std::fabs(r-color.Or) + std::fabs(g-color.Og) + std::fabs(b-color.Ob)))
        {
            mais_proximo = (std::fabs(r-color.Or) + std::fabs(g-color.Og) + std::fabs(b-color.Ob));
            c = 'O';
        }

//        std::cout << "Y " << (std::fabs(r-13.246) + std::fabs(g-109.429) + std::fabs(b-109.657)) << std::endl;
        if(mais_proximo > (std::fabs(r-color.Yr) + std::fabs(g-color.Yg) + std::fabs(b-color.Yb)))
        {
            mais_proximo = (std::fabs(r-color.Yr) + std::fabs(g-color.Yg) + std::fabs(b-color.Yb));
            c = 'Y';
        }

//        std::cout << "W " << (std::fabs(r-88.0557) + std::fabs(g-113.142) + std::fabs(b-91.4851)) << std::endl;
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

//static void help()
//{
//    cout <<
//    "\nA program using pyramid scaling, Canny, contours, contour simpification and\n"
//    "memory storage to find squares in a list of images\n"
//    "Returns sequence of squares detected on the image.\n"
//    "the sequence is stored in the specified memory storage\n"
//    "Call:\n"
//    "./squares\n"
//    "Using OpenCV version %s\n" << CV_VERSION << "\n" << endl;
//}


int thresh = 50, N = 10;
const char* wndname = "After Detection";

// helper function:
// finds a cosine of angle between vectors
// from pt0->pt1 and from pt0->pt2
static double angle( Point pt1, Point pt2, Point pt0 )
{
    double dx1 = pt1.x - pt0.x;
    double dy1 = pt1.y - pt0.y;
    double dx2 = pt2.x - pt0.x;
    double dy2 = pt2.y - pt0.y;
    return (dx1*dx2 + dy1*dy2)/sqrt((dx1*dx1 + dy1*dy1)*(dx2*dx2 + dy2*dy2) + 1e-10);
}

// returns sequence of squares detected on the image.
// the sequence is stored in the specified memory storage
static void findSquares( const Mat& image, vector<vector<Point> >& squares )
{
    squares.clear();

//s    Mat pyr, timg, gray0(image.size(), CV_8U), gray;

    // down-scale and upscale the image to filter out the noise
    //pyrDown(image, pyr, Size(image.cols/2, image.rows/2));
    //pyrUp(pyr, timg, image.size());


    // blur will enhance edge detection
    Mat timg(image);
    medianBlur(image, timg, 9);
    Mat gray0(timg.size(), CV_8U), gray;
    int x_atual_1, x_atual_2, y_atual_1, y_atual_2, repetido = 0, media_x, media_y, maior_x,menor_x,maior_y,menor_y, maior_x_atual,menor_x_atual,maior_y_atual,menor_y_atual;

    vector<vector<Point> > contours;

    // find squares in every color plane of the image
    for( int c = 0; c < 3; c++ )
    {
        int ch[] = {c, 0};
        mixChannels(&timg, 1, &gray0, 1, ch, 1);

        // try several threshold levels
        for( int l = 0; l < N; l++ )
        {
            // hack: use Canny instead of zero threshold level.
            // Canny helps to catch squares with gradient shading
            if( l == 0 )
            {
                // apply Canny. Take the upper threshold from slider
                // and set the lower to 0 (which forces edges merging)
                Canny(gray0, gray, 5, thresh, 5);
                // dilate canny output to remove potential
                // holes between edge segments
                dilate(gray, gray, Mat(), Point(-1,-1));
            }
            else
            {
                // apply threshold if l!=0:
                //     tgray(x,y) = gray(x,y) < (l+1)*255/N ? 255 : 0
                gray = gray0 >= (l+1)*255/N;
            }

            // find contours and store them all as a list
            findContours(gray, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);

            vector<Point> approx;

            // test each contour
            for( size_t i = 0; i < contours.size(); i++ )
            {
                // approximate contour with accuracy proportional
                // to the contour perimeter
                approxPolyDP(Mat(contours[i]), approx, arcLength(Mat(contours[i]), true)*0.02, true);

                // square contours should have 4 vertices after approximation
                // relatively large area (to filter out noisy contours)
                // and be convex.
                // Note: absolute value of an area is used because
                // area may be positive or negative - in accordance with the
                // contour orientation
                if( approx.size() == 4 &&
                    fabs(contourArea(Mat(approx))) > 1000 &&
                    isContourConvex(Mat(approx)) )
                {
                    double maxCosine = 0;

                    for( int j = 2; j < 5; j++ )
                    {
                        // find the maximum cosine of the angle between joint edges
                        double cosine = fabs(angle(approx[j%4], approx[j-2], approx[j-1]));
                        maxCosine = MAX(maxCosine, cosine);
                    }

                    // if cosines of all angles are small
                    // (all angles are ~90 degree) then write quandrange
                    // vertices to resultant sequence
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
                            //if(squares[i][0].x < x_atual_1 && squares[i][0].y < y_atual_1 && squares[i][3].x > x_atual_2 && squares[i][3].y > y_atual_2)
                            //std::cout << "entra aqui" << std::endl;
                            if(media_x > menor_x && media_x < maior_x &&
                                media_y > menor_y && media_y < maior_y)
                            {
//                                std::cout << "entra aqui" << std::endl;
                                repetido = 1;
                                break;
                            }
//                            if(std::abs(maior_x_atual - maior_x) < 10 ||
//                                    std::abs(maior_y_atual - maior_y) < 10 ||
//                                    std::abs(menor_x_atual - menor_x) < 10 ||
//                                    std::abs(menor_x_atual - menor_x) < 10 )
//                            {
//                                std::cout << "entra aqui" << std::endl;
//                                repetido = 1;
//                                break;
//                            }
//                            else if(maior_x_atual < maior_x && menor_x_atual > menor_x ||
//                                    maior_y_atual < maior_y && menor_y_atual > menor_y)
//                            {
//                                std::cout << "entra aqui" << std::endl;
//                                repetido = 1;
//                                break;
//                            }
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


// the function draws all the squares in the image
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
        //dont detect the border
        if (p-> x > 3 && p->y > 3)
        {
            pts[0][0] = Point(squares[i][0].x, squares[i][0].y);
            pts[0][1] = Point(squares[i][1].x, squares[i][1].y);
            pts[0][2] = Point(squares[i][2].x, squares[i][2].y);
            pts[0][3] = Point(squares[i][3].x, squares[i][3].y);
            const Point* points[1] = {pts[0]};
            //std::cout << points[0][0].x << std::endl;
            maior_x = std::max(squares[i][0].x, std::max(squares[i][1].x , std::max(squares[i][2].x, squares[i][3].x)));
            maior_y = std::max(squares[i][0].y, std::max(squares[i][1].y , std::max(squares[i][2].y, squares[i][3].y)));
            menor_x = std::min(squares[i][0].x, std::min(squares[i][1].x , std::min(squares[i][2].x, squares[i][3].x)));
            menor_y = std::min(squares[i][0].y, std::min(squares[i][1].y , std::min(squares[i][2].y, squares[i][3].y)));

            cv::Mat miniMat = cv::Mat(image, cv::Rect(menor_x, menor_y, maior_x-menor_x, maior_y-menor_y));

            //imshow(wndname, miniMat);
            //cvWaitKey(0);
            //Mat1b mask(miniMat.rows, miniMat.cols, uchar(0));
            //fillPoly(mask, points, &npoints, 1, Scalar(255,255,255));
            average = mean(miniMat);
            r = average[0];
            g = average[1];
            b = average[2];

            if(squares.size() == 9)
            {
//                std::cout << "result size: " << result.size() << std::endl;
                result[i*3] = r;
                result[i*3+1] = g;
                result[i*3+2] = b;
            }
//            std::cout << "aqui" << std::endl;
            c = teste::detectColor(r,g,b,conf);
            colors.push_back(teste::Char2StyleSheet(c));
//            std::cout << c << i+1 << std::endl;
//            std::cout << "x: " << menor_x << " y: " << menor_y << std::endl;
//            std::cout << menor_y*2 + menor_x << std::endl;

            polylines(image, &p, &n, 1, true, teste::colorForLine(c), 3, LINE_AA);
        }

    }


//    std::cout << average << std::endl;
    imshow(wndname, image);

    return result;
}




//int main(int /*argc*/, char** /*argv*/)
//{
//    static const char* names[] = { "imgs/2Stickies.jpg", "imgs/manyStickies.jpg",0 };
//    help();
//    namedWindow( wndname, 1 );
//    vector<vector<Point> > squares;

//    for( int i = 0; names[i] != 0; i++ )
//    {
        //Mat image = imread(names[i], 1);
//        if( image.empty() )
//        {
//            cout << "Couldn't load " << names[i] << endl;
//            continue;
//        }

//        findSquares(image, squares);
//        drawSquares(image, squares);
//        //imwrite( "out", image );
//        int c = waitKey();
//        if( (char)c == 27 )
//            break;
//    }

//    return 0;
//}
