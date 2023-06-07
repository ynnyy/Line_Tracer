#include "opencv2/opencv.hpp"
#include <iostream>
#include <signal.h>
#include "vision.hpp"
using namespace std;
using namespace cv;

int main(void)
{

    VideoCapture source("linetracer100rpm outcourse.mp4");
    if (!source.isOpened()) { cerr << "Video error" << endl; return -1; }

    int vel1 = 0, vel2 = 0;
    Mat frame, roi_value, img_labels, stats, centroids, line;
    Point2f line_center;
    Point2f  prevLineCenter(source.get(CAP_PROP_FRAME_WIDTH) / 2, source.get(CAP_PROP_FRAME_HEIGHT) * 3 / 4); //영상의 중심을 초기 라인의 중심으로 저장
    int preLineLabel = -1;
   // Point2f  prevLineCenter(source.rows / 2, source.cols *3/ 4); //영상의 중심을 초기 라인의 중심으로 저장
    
    while (true)
    {
        source >> frame;
        resize(frame, frame, Size(640, 360));//동영상으로연습시만
        if (frame.empty()) {
            cerr << "frame empty!" << endl;
            break;
        }

        //전처리 함수 호출
        roi_value = ROI(frame);
        //라인 검출
        
        //line = line_detect(roi_value, centroid, cnt)
        Mat get_line =line_detect(roi_value,&prevLineCenter,&line_center);
        double error = get_error(roi_value, line_center);

        imshow("img", frame);
        imshow("img_cvt", roi_value);
        imshow("line", get_line);

        if (waitKey(1) == 27) break;


        //diff1 = end1.tv_sec + end1.tv_usec / 1000000.0 - start.tv_sec - start.tv_usec / 1000000.0;
        //cout << "vel1:" << vel1 << ',' << "vel2:" << vel2 << endl;
        //cout << "time:" << diff1 << endl;
    }

    return 0;

}
