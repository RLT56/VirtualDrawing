#include <sstream>
#include "opencv2/opencv.hpp" 
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include "utils.h"

using namespace cv;
using namespace std;
extern int H_MIN, H_MAX, S_MIN, S_MAX, V_MIN, V_MAX, FINISHED;
extern const int FRAME_WIDTH, FRAME_HEIGHT, MAX_NUM_OBJECTS, MIN_OBJECT_AREA, MAX_OBJECT_AREA;
extern const string windowName, windowName1, windowName2, windowName3, trackbarWindowName;

// Draws a target at a specific position
void drawObject(int x, int y, Mat& frame) {

	circle(frame, Point(x, y), 5, Scalar(0, 255, 0), 2);
	if (y - 25 > 0)
		line(frame, Point(x, y), Point(x, y - 25), Scalar(0, 255, 0), 2);
	else line(frame, Point(x, y), Point(x, 0), Scalar(0, 255, 0), 2);
	if (y + 25 < FRAME_HEIGHT)
		line(frame, Point(x, y), Point(x, y + 25), Scalar(0, 255, 0), 2);
	else line(frame, Point(x, y), Point(x, FRAME_HEIGHT), Scalar(0, 255, 0), 2);
	if (x - 25 > 0)
		line(frame, Point(x, y), Point(x - 25, y), Scalar(0, 255, 0), 2);
	else line(frame, Point(x, y), Point(0, y), Scalar(0, 255, 0), 2);
	if (x + 25 < FRAME_WIDTH)
		line(frame, Point(x, y), Point(x + 25, y), Scalar(0, 255, 0), 2);
	else line(frame, Point(x, y), Point(FRAME_WIDTH, y), Scalar(0, 255, 0), 2);

	putText(frame, intToString(x) + "," + intToString(y), Point(x, y + 30), 1, 1, Scalar(0, 255, 0), 2);

}

void drawObject(int x, int y, Mat& frame, Mat& draw) {

	circle(frame, Point(x, y), 5, Scalar(0, 255, 0), 2);
	if (y - 25 > 0)
		line(frame, Point(x, y), Point(x, y - 25), Scalar(0, 255, 0), 2);
	else line(frame, Point(x, y), Point(x, 0), Scalar(0, 255, 0), 2);
	if (y + 25 < FRAME_HEIGHT)
		line(frame, Point(x, y), Point(x, y + 25), Scalar(0, 255, 0), 2);
	else line(frame, Point(x, y), Point(x, FRAME_HEIGHT), Scalar(0, 255, 0), 2);
	if (x - 25 > 0)
		line(frame, Point(x, y), Point(x - 25, y), Scalar(0, 255, 0), 2);
	else line(frame, Point(x, y), Point(0, y), Scalar(0, 255, 0), 2);
	if (x + 25 < FRAME_WIDTH)
		line(frame, Point(x, y), Point(x + 25, y), Scalar(0, 255, 0), 2);
	else line(frame, Point(x, y), Point(FRAME_WIDTH, y), Scalar(0, 255, 0), 2);
	
	circle(draw, Point(x, y), 5, Scalar(255, 255, 255), -1);


	putText(frame, intToString(x) + "," + intToString(y), Point(x, y + 30), 1, 1, Scalar(0, 255, 0), 2);

}

// Performs morphological operations
void morphOps(Mat& thresh) {

	Mat erodeElement = getStructuringElement(MORPH_RECT, Size(3, 3));
	Mat dilateElement = getStructuringElement(MORPH_RECT, Size(8, 8));

	erode(thresh, thresh, erodeElement, Point(-1, -1), 2);
	dilate(thresh, thresh, dilateElement, Point(-1, -1), 2);

}

void trackFilteredObject(int& x, int& y, Mat& threshold, Mat& cameraFeed, Mat& draw) {

	Mat temp;
	threshold.copyTo(temp);
    const int MAX_OBJECT_AREA = FRAME_HEIGHT * FRAME_WIDTH / 1.5;
	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;

	// Finds contours of filtered image 
	findContours(temp, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);

	// Finds the filtered object with moments method 
	double refArea = 0;
	bool objectFound = false;
	if (hierarchy.size() > 0) {
		int numObjects = hierarchy.size();

		// Continue if there are not too many objects
		if (numObjects < MAX_NUM_OBJECTS) {
			for (int index = 0; index >= 0; index = hierarchy[index][0]) {

				Moments moment = moments((cv::Mat)contours[index]);
				double area = moment.m00;

				if (area > MIN_OBJECT_AREA && area<MAX_OBJECT_AREA && area>refArea) {
					x = moment.m10 / area;
					y = moment.m01 / area;
					objectFound = true;
					refArea = area;
				}
				else objectFound = false;


			}
			// An object is found
			if (objectFound == true) {
				putText(cameraFeed, "Tracking Object", Point(0, 50), 2, 1, Scalar(0, 255, 0), 2);
				// Draws object location on screen
				drawObject(x, y, cameraFeed, draw);
			}

		}
		
		// If there are too many objects, the filter is too noisy
		else putText(cameraFeed, "TOO MUCH NOISE! ADJUST FILTER", Point(0, 50), 1, 2, Scalar(0, 0, 255), 2);
	}
}

int drawPoints(VideoCapture& capture, Mat& cameraFeed, Mat& HSV, Mat& threshold, Mat& draw, int x, int y)
{
while (1) {

		capture.read(cameraFeed);

		flip(cameraFeed, cameraFeed, 1);

		cvtColor(cameraFeed, HSV, COLOR_BGR2HSV);

		inRange(HSV, Scalar(H_MIN, S_MIN, V_MIN), Scalar(H_MAX, S_MAX, V_MAX), threshold);

		morphOps(threshold);

		trackFilteredObject(x, y, threshold, cameraFeed, draw);

		imshow("Original Image", cameraFeed); 
		imshow("Draw", draw);

		if (waitKey(30) == 27) {
			return 0;
		}
}}