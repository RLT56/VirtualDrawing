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
void morphOps(Mat& thresh) {

	Mat erodeElement = getStructuringElement(MORPH_RECT, Size(3, 3));
	Mat dilateElement = getStructuringElement(MORPH_RECT, Size(8, 8));

	erode(thresh, thresh, erodeElement);
	erode(thresh, thresh, erodeElement);


	dilate(thresh, thresh, dilateElement);
	dilate(thresh, thresh, dilateElement);

}
void trackFilteredObject(int& x, int& y, Mat threshold, Mat& cameraFeed) {

	Mat temp;
	threshold.copyTo(temp);
    const int MAX_OBJECT_AREA = FRAME_HEIGHT * FRAME_WIDTH / 1.5;
	//these two vectors needed for output of findContours
	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;
	//find contours of filtered image using openCV findContours function
	findContours(temp, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
	//use moments method to find our filtered object
	double refArea = 0;
	bool objectFound = false;
	if (hierarchy.size() > 0) {
		int numObjects = hierarchy.size();
		//if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
		if (numObjects < MAX_NUM_OBJECTS) {
			for (int index = 0; index >= 0; index = hierarchy[index][0]) {

				Moments moment = moments((cv::Mat)contours[index]);
				double area = moment.m00;

				//if the area is less than 20 px by 20px then it is probably just noise
				//if the area is the same as the 3/2 of the image size, probably just a bad filter
				//we only want the object with the largest area so we safe a reference area each
				//iteration and compare it to the area in the next iteration.
				if (area > MIN_OBJECT_AREA && area<MAX_OBJECT_AREA && area>refArea) {
					x = moment.m10 / area;
					y = moment.m01 / area;
					objectFound = true;
					refArea = area;
				}
				else objectFound = false;


			}
			//let user know you found an object
			if (objectFound == true) {
				putText(cameraFeed, "Tracking Object", Point(0, 50), 2, 1, Scalar(0, 255, 0), 2);
				//draw object location on screen
				drawObject(x, y, cameraFeed);
			}

		}
		else putText(cameraFeed, "TOO MUCH NOISE! ADJUST FILTER", Point(0, 50), 1, 2, Scalar(0, 0, 255), 2);
	}
}

int drawPoints(VideoCapture capture, Mat& cameraFeed, Mat& HSV, Mat& threshold, Mat& draw, int x, int y)
{
while (1) {
		capture.read(cameraFeed);

		//flip image
		flip(cameraFeed, cameraFeed, 1);

		cvtColor(cameraFeed, HSV, COLOR_BGR2HSV);

		inRange(HSV, Scalar(H_MIN, S_MIN, V_MIN), Scalar(H_MAX, S_MAX, V_MAX), threshold);

		Mat erodeElement = getStructuringElement(MORPH_RECT, Size(3, 3));
		Mat dilateElement = getStructuringElement(MORPH_RECT, Size(8, 8));
		erode(threshold, threshold, erodeElement);
		erode(threshold, threshold, erodeElement);
		dilate(threshold, threshold, dilateElement);
		dilate(threshold, threshold, dilateElement);

		Mat temp;
		threshold.copyTo(temp);
		vector< vector<Point> > contours;
		vector<Vec4i> hierarchy;
		findContours(temp, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);

		double refArea = 0;
		bool objectFound = false;

		if (hierarchy.size() > 0) {
			int numObjects = hierarchy.size();
			//if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
			if (numObjects < MAX_NUM_OBJECTS) {
				for (int index = 0; index >= 0; index = hierarchy[index][0]) {
					Moments moment = moments((cv::Mat)contours[index]);
					double area = moment.m00;

					if (area > MIN_OBJECT_AREA && area<MAX_OBJECT_AREA && area>refArea) {
						x = moment.m10 / area;
						y = moment.m01 / area;
						objectFound = true;
						refArea = area;
						cout << "X: " << x / 2 << " Y: " << y / 2 << "\n";
					}
					else objectFound = false;
				}
				//let user know you found an object
				if (objectFound == true) {
					putText(cameraFeed, "Tracking Object", Point(0, 50), 2, 1, Scalar(0, 255, 0), 2);

					
					circle(cameraFeed, Point(x, y), 20, Scalar(0, 255, 0), 2);
					if (y - 25 > 0)
						line(cameraFeed, Point(x, y), Point(x, y - 25), Scalar(0, 255, 0), 2);
					else line(cameraFeed, Point(x, y), Point(x, 0), Scalar(0, 255, 0), 2);
					if (y + 25 < FRAME_HEIGHT)
						line(cameraFeed, Point(x, y), Point(x, y + 25), Scalar(0, 255, 0), 2);
					else line(cameraFeed, Point(x, y), Point(x, FRAME_HEIGHT), Scalar(0, 255, 0), 2);
					if (x - 25 > 0)
						line(cameraFeed, Point(x, y), Point(x - 25, y), Scalar(0, 255, 0), 2);
					else line(cameraFeed, Point(x, y), Point(0, y), Scalar(0, 255, 0), 2);
					if (x + 25 < FRAME_WIDTH)
						line(cameraFeed, Point(x, y), Point(x + 25, y), Scalar(0, 255, 0), 2);
					else line(cameraFeed, Point(x, y), Point(FRAME_WIDTH, y), Scalar(0, 255, 0), 2);
					
					circle(draw, Point(x, y), 5, Scalar(255, 255, 255), -1);
				}

			}
			else putText(cameraFeed, "TOO MUCH NOISE! ADJUST FILTER", Point(0, 50), 1, 2, Scalar(0, 0, 255), 2);
		}

		imshow("Original Image", cameraFeed); 
		imshow("Draw", draw);

		if (waitKey(30) == 27) {
			return 0;
		}
}}