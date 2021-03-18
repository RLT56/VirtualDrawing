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
extern int H_MIN, H_MAX, S_MIN, S_MAX, V_MIN, V_MAX, FINISHED;
extern const int FRAME_WIDTH, FRAME_HEIGHT, MAX_NUM_OBJECTS, MIN_OBJECT_AREA, MAX_OBJECT_AREA;
extern const string windowName, windowName1, windowName2, windowName3, trackbarWindowName;

void on_trackbar(int, void*)
{}
string intToString(int number){
	std::stringstream ss;
	ss << number;
	return ss.str();
}

// Create window for trackbars
void createTrackbars() {

	namedWindow("Trackbars", 0);
	char TrackbarName[50];
	sprintf(TrackbarName, "H_MIN");
	sprintf(TrackbarName, "H_MAX");
	sprintf(TrackbarName, "S_MIN");
	sprintf(TrackbarName, "S_MAX");
	sprintf(TrackbarName, "V_MIN");
	sprintf(TrackbarName, "V_MAX");
	sprintf(TrackbarName, "FINISH");
	// The user moves the slider
	// The HSV values are automatically modified   
	createTrackbar("H_MIN", "Trackbars", &H_MIN, H_MAX, on_trackbar);
	createTrackbar("H_MAX", "Trackbars", &H_MAX, H_MAX, on_trackbar);
	createTrackbar("S_MIN", "Trackbars", &S_MIN, S_MAX, on_trackbar);
	createTrackbar("S_MAX", "Trackbars", &S_MAX, S_MAX, on_trackbar);
	createTrackbar("V_MIN", "Trackbars", &V_MIN, V_MAX, on_trackbar);
	createTrackbar("V_MAX", "Trackbars", &V_MAX, V_MAX, on_trackbar);
	createTrackbar("Swith when finished", "Trackbars", &FINISHED, 1, on_trackbar);
	
}

// Asks the user to input the HSV values
void askMenu(int &H_MIN, int &H_MAX, int &S_MIN, int &S_MAX, int &V_MIN, int &V_MAX)
{ cout<<"H_MIN = ?"<<endl;
  cin>>H_MIN;
  cout<<"H_MAX = ?"<<endl;
  cin>>H_MAX;
  cout<<"S_MIN = ?"<<endl;
  cin>>S_MIN;
  cout<<"S_MAX = ?"<<endl;
  cin>>S_MAX;
  cout<<"V_MIN = ?"<<endl;
  cin>>V_MIN;
  cout<<"V_MAX = ?"<<endl;
  cin>>V_MAX;
}

// Performs calibration
void calibrate(VideoCapture& capture, Mat& cameraFeed, Mat& HSV, Mat& threshold, int x, int y, bool trackObjects, bool useMorphOps)
{
    while(1){

		capture.read(cameraFeed);
		cvtColor(cameraFeed, HSV, COLOR_BGR2HSV);
		// Filters HSV image between values and store filtered image to threshold matrix
		inRange(HSV, Scalar(H_MIN, S_MIN, V_MIN), Scalar(H_MAX, S_MAX, V_MAX), threshold);
		// Morphological operations to eliminate noise and emphasize the filtered object
		if (useMorphOps)
			morphOps(threshold);
		// Extract the coordinates of the target object
		if (trackObjects)
			trackFilteredObject(x, y, threshold, cameraFeed, HSV);

		// Windows
		imshow(windowName2, threshold);
		imshow(windowName, cameraFeed);
		imshow(windowName1, HSV);

		waitKey(30);

		// The process ends whan the user toggles the last slider
		if (FINISHED) {cv::destroyWindow(windowName1);
					   cv::destroyWindow(windowName2);
			           break;}}

}