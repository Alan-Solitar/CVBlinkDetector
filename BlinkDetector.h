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
	static BlinkDetector *bt;
	BlinkDetector();
	static bool eyeStatus;
	static Mat eyeOne;
	static Mat eyeTwo;
	static BlinkDetector *GetInstance()
	{
		if (!bt)
			bt = new BlinkDetector;
		return bt;
	}
	bool OpenEyeDetectedFromTemplate(Mat &image, Mat &resultImage, bool firstTemplate = true);
	bool CheckForBlink(Mat &image, Mat leftEye,Mat rightEye, Mat &resultImage);


};

#endif