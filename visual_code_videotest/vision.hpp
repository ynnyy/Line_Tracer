
#ifndef _VISION_HPP_
#define _VISION_HPP_
using namespace cv;
#include "opencv2/opencv.hpp"
Mat line_detect(Mat frame, Point2f* prevLineCenter, Point2f*);
Mat ROI(Mat frame);
double get_error(Mat , Point2f );
#endif