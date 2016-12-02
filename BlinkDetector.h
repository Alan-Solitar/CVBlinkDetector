#pragma once
#ifndef BLINK_DETECTOR_H
#define BLINK_DETECTOR_H

#include <opencv2/core/core.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\opencv.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\objdetect.hpp>
#include <iostream>

using namespace cv;
using namespace std;

class BlinkDetector
{
public:
	static uint blinkCounter;
	
	BlinkDetector();
	static pair<bool,bool> eyeStatus;
	static Mat eyeOne;
	static Mat eyeTwo;
	pair<Point,Point> prevLocation;
	static BlinkDetector* GetInstance()
	{
		if (!BlinkDetector::bt)
			BlinkDetector::bt = new BlinkDetector;
		return BlinkDetector::bt;
	}
	bool OpenEyeDetectedFromTemplate(Mat &image, Mat &resultImage, bool firstTemplate = true);
	//bool CheckForBlink(Mat &image, Mat leftEye,Mat rightEye, Mat &resultImage);
private:
	static BlinkDetector *bt;

};

#endif