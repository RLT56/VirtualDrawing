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

//initial min and max HSV filter values.
//these will be changed using trackbars
int H_MIN = 0;
int H_MAX = 256;
int S_MIN = 0;
int S_MAX = 256;
int V_MIN = 0;
int V_MAX = 256;
int FINISHED = 0;
//default capture width and height
const int FRAME_WIDTH = 640;
const int FRAME_HEIGHT = 480;
//max number of objects to be detected in frame
const int MAX_NUM_OBJECTS = 50;
//minimum and maximum object area
const int MIN_OBJECT_AREA = 20 * 20;
const int MAX_OBJECT_AREA = FRAME_HEIGHT * FRAME_WIDTH / 1.5;
//names that will appear at the top of each window
const string windowName = "Original Image";
const string windowName1 = "HSV Image";
const string windowName2 = "Thresholded Image";
const string windowName3 = "After Morphological Operations";
const string trackbarWindowName = "Trackbars";

int main(int argc, char* argv[])
{
	bool trackObjects = false;
	bool useMorphOps = false;
	//Matrix to store each frame of the webcam feed
	Mat cameraFeed;
	//matrix storage for HSV image
	Mat HSV;
	//matrix storage for binary threshold image
	Mat threshold;
	//x and y values for the location of the object
	int x = 0, y = 0;
	char choice;
	VideoCapture capture;
	std::cout<<"Do you need to calibrate ? (y/n)";
	cin>>choice;
	if (choice != 'n' && choice !='N')
	{
	//create slider bars for HSV filtering
	createTrackbars();
	//video capture object to acquire webcam feed
	
	//open capture object at location zero (default location for webcam)
	capture.open(0);
	//set height and width of capture frame
	capture.set(CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
	capture.set(CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);
	//start an infinite loop where webcam feed is copied to cameraFeed matrix
	//all of our operations will be performed within this loop
	calibrate(capture, cameraFeed, HSV, threshold, x, y, trackObjects, useMorphOps);

	}
	else askMenu(H_MIN, H_MAX, S_MIN, S_MAX, V_MIN, V_MAX);

	// Begin Drawing

	//declare the drawing window
	Mat draw = Mat::zeros(FRAME_HEIGHT, FRAME_WIDTH, CV_8UC3);;

	x = 0, y = 0;

	//capture from webcam
	capture.open(0);

	capture.set(CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
	capture.set(CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);

	drawPoints(capture, cameraFeed, HSV, threshold, draw, x, y);

	return 0;
}
 // 60 95 119 256 111 239