#include <sstream>
#include "opencv2/opencv.hpp" 
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace cv;


void on_trackbar(int, void*);

std::string intToString(int number);

void createTrackbars();

void askMenu(int &H_MIN, int &H_MAX, int &S_MIN, int &S_MAX, int &V_MIN, int &V_MAX);

void calibrate(VideoCapture capture, Mat cameraFeed, Mat HSV, Mat threshold, int x, int y, bool trackObjects, bool useMorphOps);


