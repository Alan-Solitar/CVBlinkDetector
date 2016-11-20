#pragma once

#ifndef BLINK_MANAGER_H
#define BLINK_MANAGER_H

#include <opencv2/core/core.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\opencv.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\objdetect.hpp>
#include <iostream>
using namespace cv;
using namespace std;

//Class to handle all functionality related to Blink Detection
class BlinkManager {
	//vars
private:
	BlinkManager* bManager;
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



};

#endif