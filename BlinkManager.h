#pragma once

#ifndef BLINK_MANAGER_H
#define BLINK_MANAGER_H

#include <opencv2/core/core.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\opencv.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\objdetect.hpp>
#include <iostream>
#include "BlinkDetector.h"
using namespace cv;
using namespace std;

//Class to handle all functionality related to Blink Detection
class BlinkManager 
{
private:
	BlinkDetector* blinkDetector;
	VideoCapture capture;
	cv::CascadeClassifier faceCascade;
	cv::CascadeClassifier eyeCascade;
	
public:
	BlinkManager(){}
	BlinkManager(const string &faceCascadeFile, const string eyeCascadeFile);
	bool InitCamera();
	bool Init();
	bool SetCascades(const string &faceCascadeFile, const string eyeCascadeFile);
	void RunBlinkDetector();
	bool basicDetection(Mat &prevGray, Rect &face, vector<Point2f> &points);
	bool CheckForOutOfBoundsPoints(vector<Point2f> &points, int &rows, int &columns);
	void DisplayMessage(Mat &image, const string &message, Scalar color, int x, int y);
	bool CheckEyeStatus(Mat &frame, Mat &processedFrame);
	bool LogEyeStatus(bool status);
	bool HandleBlink(bool blinked, Mat &frame);
	const string BLINK_MSG = "Blink Detected";
	const string BLINK_COUNTER_MSG_BASE = "Count: ";
	string BLINK_COUNTER_MSG = BLINK_COUNTER_MSG_BASE;


};

#endif