#include <sstream>
#include "opencv2/opencv.hpp" 
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include "utils.h"
#include "Drawing.h"

using namespace cv;
using namespace std;

// Initial values for the HSV thresholds
int H_MIN = 0;
int H_MAX = 256;
int S_MIN = 0;
int S_MAX = 256;
int V_MIN = 0;
int V_MAX = 256;
int FINISHED = 0;

// Stream dimensions
const int FRAME_WIDTH = 640;
const int FRAME_HEIGHT = 480;

// Maximum number of objects in a frame
const int MAX_NUM_OBJECTS = 50;

// Dimensions of the object detection area
const int MIN_OBJECT_AREA = 20 * 20;
const int MAX_OBJECT_AREA = FRAME_HEIGHT * FRAME_WIDTH / 1.5;

// Window names
const string windowName = "Original Image";
const string windowName1 = "HSV Image";
const string windowName2 = "Thresholded Image";
const string windowName3 = "After Morphological Operations";
const string trackbarWindowName = "Trackbars";

int main(int argc, char* argv[])
{
	bool trackObjects = false;
	bool useMorphOps = false;

	// Image matrices
	Mat cameraFeed;
	Mat HSV;
	Mat threshold;

	VideoCapture capture;

	// Object location
	int x = 0, y = 0;

	// Calibration
	char choice;
	std::cout<<"Do you need to calibrate ? (y/n)";
	cin>>choice;
	if (choice != 'n' && choice !='N'){
	// Display trackbars to select HSV values
	createTrackbars();
	
	// Open video capture
	capture.open(0);
	capture.set(CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
	capture.set(CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);

	// The user moves the sliders until only the target appears in white
	// When finished the user must toggle the finished slider
	calibrate(capture, cameraFeed, HSV, threshold, x, y, trackObjects, useMorphOps);

	}

	// Asks the user to directly provide the HSV values
	else askMenu(H_MIN, H_MAX, S_MIN, S_MAX, V_MIN, V_MAX);

	// Begin Drawing

	Mat draw = Mat::zeros(FRAME_HEIGHT, FRAME_WIDTH, CV_8UC3);;
	x = 0, y = 0;

	// Webcam capture
	capture.open(0);
	capture.set(CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
	capture.set(CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);

	// Draws points along the target trajectory
	drawPoints(capture, cameraFeed, HSV, threshold, draw, x, y);

	return 0;
}
 // 60 95 119 256 111 239