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

void on_trackbar(int, void*)
{}
string intToString(int number){
	std::stringstream ss;
	ss << number;
	return ss.str();
}

void createTrackbars(int &H_MIN, int &H_MAX, int &S_MIN, int &S_MAX, int &V_MIN, int &V_MAX, int &FINISHED) {
	//create window for trackbars


	namedWindow("Trackbars", 0);
	//create memory to store trackbar name on window
	char TrackbarName[50];
	sprintf(TrackbarName, "H_MIN");
	sprintf(TrackbarName, "H_MAX");
	sprintf(TrackbarName, "S_MIN");
	sprintf(TrackbarName, "S_MAX");
	sprintf(TrackbarName, "V_MIN");
	sprintf(TrackbarName, "V_MAX");
	sprintf(TrackbarName, "FINISH");
	//create trackbars and insert them into window
	//3 parameters are: the address of the variable that is changing when the trackbar is moved(eg.H_LOW),
	//the max value the trackbar can move (eg. H_HIGH), 
	//and the function that is called whenever the trackbar is moved(eg. on_trackbar)
	//                                  ---->    ---->     ---->      
	createTrackbar("H_MIN", "Trackbars", &H_MIN, H_MAX, on_trackbar);
	createTrackbar("H_MAX", "Trackbars", &H_MAX, H_MAX, on_trackbar);
	createTrackbar("S_MIN", "Trackbars", &S_MIN, S_MAX, on_trackbar);
	createTrackbar("S_MAX", "Trackbars", &S_MAX, S_MAX, on_trackbar);
	createTrackbar("V_MIN", "Trackbars", &V_MIN, V_MAX, on_trackbar);
	createTrackbar("V_MAX", "Trackbars", &V_MAX, V_MAX, on_trackbar);
	createTrackbar("Swith when finished", "Trackbars", &FINISHED, 1, on_trackbar);
	
}

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





/*class HSV
{
    int H_MIN; 
    int H_MAX; 
    int S_MIN;
    int S_MAX; 
    int V_MIN;
    int V_MAX;
    int FINISHED;

    public:

    HSV(int H_MIN, int H_MAX, int S_MIN, int S_MAX, int V_MIN, int V_MAX, bool FINISHED)
    {
        this->H_MIN = H_MIN;
        this->H_MAX = H_MAX;
        this->S_MIN = S_MIN;
        this->S_MAX = S_MAX;
        this->V_MIN = V_MIN;
        this->V_MAX = V_MAX;

    }

    int get_HMIN() {return H_MIN;}
    int get_HMAX(){return H_MAX;}
    int get_SMIN(){return S_MIN;}
    int get_SMAX(){return S_MAX;}
    int get_VMIN(){return V_MIN;}
    int get_VMAX(){return V_MAX;}


};*/