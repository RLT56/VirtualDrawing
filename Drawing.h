#include <sstream>
#include "opencv2/opencv.hpp" 
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <stdio.h>
using namespace std;
extern int H_MIN, H_MAX, S_MIN, S_MAX, V_MIN, V_MAX, FINISHED;
extern const int FRAME_WIDTH, FRAME_HEIGHT, MAX_NUM_OBJECTS, MIN_OBJECT_AREA, MAX_OBJECT_AREA;
extern const string windowName, windowName1, windowName2, windowName3, trackbarWindowName;

void drawObject(int x, int y, cv::Mat& frame);
void drawObject(int x, int y, cv::Mat& frame, cv::Mat& draw);
void morphOps(cv::Mat& thresh);

void trackFilteredObject(int& x, int& y, cv::Mat& threshold, cv::Mat& cameraFeed, cv::Mat& draw);


int drawPoints(cv::VideoCapture& capture, cv::Mat& cameraFeed, cv::Mat& HSV, cv::Mat& threshold, cv::Mat& draw, int x, int y);