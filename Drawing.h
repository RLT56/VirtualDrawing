#include <sstream>
#include "opencv2/opencv.hpp" 
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <stdio.h>

void drawObject(int x, int y, cv::Mat& frame, const int FRAME_HEIGHT, const int FRAME_WIDTH);

void morphOps(cv::Mat& thresh);

void trackFilteredObject(int& x, int& y, cv::Mat threshold, cv::Mat& cameraFeed,  const int MAX_NUM_OBJECTS, const int MAX_OBJECT_AREA,const int FRAME_HEIGHT, const int FRAME_WIDTH);