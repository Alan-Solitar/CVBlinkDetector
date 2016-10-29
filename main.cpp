#include <opencv2/core/core.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\opencv.hpp>
#include "BlinkManager.h"
#include <iostream>
using namespace cv;
using namespace std;


//short program which shows how to convert an image into greyscale
int main()
{
	BlinkManager blinkManager;
	blinkManager.Init();
	blinkManager.RunBlinkDetector();
	/*
	const string faceCascade = "C:\OpenCV - 3.1.0\opencv\build\etc\haarcascades";

	//set up camera
	VideoCapture capture(0);
	if (!capture.isOpened())
		cout << "No camera or problem with camera" << endl;

	//window to display camera capture
	namedWindow("videoCapture", 1);
	namedWindow("blur", 1);
	//main loop
	for (;;) 
	{
		//Get new frame from buffer
		Mat frame;
		capture >> frame;
		Mat blurred;
		//some smoothing
		GaussianBlur(frame, blurred, Size(9, 9), 1.5, 1.5);
		imshow("videoCapture", frame);
		imshow("blur", blurred);
		if (waitKey(30) >= 0) break;

	}
	*/

}