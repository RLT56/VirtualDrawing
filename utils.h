#include <sstream>
#include "opencv2/opencv.hpp" 
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <stdio.h>


void on_trackbar(int, void*);

std::string intToString(int number);

void createTrackbars(int &H_MIN, int &H_MAX, int &S_MIN, int &S_MAX, int &V_MIN, int &V_MAX, int &FINISHED);

void askMenu(int &H_MIN, int &H_MAX, int &S_MIN, int &S_MAX, int &V_MIN, int &V_MAX);

