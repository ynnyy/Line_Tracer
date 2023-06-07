#include "opencv2/opencv.hpp"
#include <iostream>
#include <signal.h>
#include "vision.hpp"
#include "dxl.hpp"
using namespace std;
using namespace cv;
int main(void)
{
    string src = "nvarguscamerasrc sensor-id=0 ! video/x-raw(memory:NVMM), width=(int)640, \
    height=(int)360, format=(string)NV12 ! \
    nvvidconv flip-method=0 ! video/x-raw, width=(int)640, height=(int)360, \
    format=(string)BGRx ! videoconvert ! \
    video/x-raw, format=(string)BGR !appsink";
    string dst = "appsrc ! videoconvert ! video/x-raw, format=BGRx ! nvvidconv ! nvv4l2h264enc \
    insert-sps-pps=true ! h264parse ! rtph264pay pt=96 ! udpsink host=203.234.58.166 \
    port=8007 sync=false";
    string dst2 = "appsrc ! videoconvert ! video/x-raw, format=BGRx ! nvvidconv ! nvv4l2h264enc \
    insert-sps-pps=true ! h264parse ! rtph264pay pt=96 ! udpsink host=203.234.58.166 \
    port=8008 sync=false";
    //VideoCapture source("linetracer50rpm.mp4");
    VideoCapture source(src,CAP_GSTREAMER); //카메라 열기
    if (!source.isOpened()) { cerr << "Video error" << endl; return -1; }
    VideoWriter writer(dst, 0, (double)30, cv::Size(640, 360), true);
    VideoWriter writer2(dst2, 0, (double)30, cv::Size(640, 90), true);  
    if (!writer.isOpened()) { cerr << "Writer open failed!" << endl; return -1;}
    double diff1;
    Dxl mx;
    struct timeval start,end1;
    Mat frame, roi_value, img_labels, stats, centroids, line;
    Point2f line_center;
    Point2f  prevLineCenter(source.get(CAP_PROP_FRAME_WIDTH) / 2, source.get(CAP_PROP_FRAME_HEIGHT) * 3 / 4); //영상의 중심을 초기 라인의 중심으로 저장
    int preLineLabel = -1;
   // Point2f  prevLineCenter(source.rows / 2, source.cols *3/ 4); //영상의 중심을 초기 라인의 중심으로 저장
    int mode=false;
    if(!mx.open()) { cout << "dynamixel open error"<<endl; return -1; } //장치열기
    while (true)
    {
        gettimeofday(&start,NULL);
        source >> frame;
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
        gettimeofday(&end1,NULL);
        writer << frame;
        writer2 <<get_line;
        int c;
        if (mx.kbhit()) //키보드입력 체크
        {
            c = mx.getch(); //키입력 받기
            if (c =='q') break; //Ctrl+c입력시 탈출
            else if(c=='s') mode=true;
        } 
        //robot_control(error,mx);
        if(mode){
            robot_control(error,mx);
        }


        //imshow("img", frame);
        //imshow("img_cvt", roi_value);
        //imshow("line", get_line);
        if (waitKey(1) == 27) break;

        diff1 = end1.tv_sec + end1.tv_usec / 1000000.0 - start.tv_sec - start.tv_usec / 1000000.0;
        cout << "time:" << diff1 << endl;
    }
    mx.close();
    return 0;

}
