//Written by Alan Solitar

#include "BlinkManager.h"
#include "BlinkDetector.h"
#include "Detections.h"
#include <exception>
#include <iostream>
#include <cstdio>
#include <ctime>
#include <iomanip>

using namespace HaarDetections;

BlinkManager::BlinkManager(const string &faceCascadeFile, const string eyeCascadeFile) {
	SetCascades(faceCascadeFile, eyeCascadeFile);
	blinkDetector = BlinkDetector::GetInstance();
	bool initSuceeded = Init();
	if (!initSuceeded)
		throw exception("Something bad happened -- maybe consider a using a camera.");
}
bool BlinkManager::SetCascades(const string &faceCascadeFile, const string eyeCascadeFile)
{
	//load face cascade
	if (!faceCascade.load(faceCascadeFile))
		return false;

	//load eye cascade
	if (!eyeCascade.load(eyeCascadeFile))
		return false;
	return true;
}

bool BlinkManager::Init() 
{
	bool allGood = true;
	allGood = InitCamera();
	return allGood;
}
bool BlinkManager::InitCamera() {
	capture = VideoCapture(0);
	return capture.isOpened();
}
bool BlinkManager::BasicDetection(Mat &prevGray, Rect &face, vector<Point2f> &points)
{
	Mat prevFrame;
	capture >> prevFrame;
	bool foundFace = DetectFace(faceCascade, eyeCascade, prevFrame, face);

	cvtColor(prevFrame, prevGray, COLOR_BGR2GRAY);
	equalizeHist(prevGray, prevGray);

	bool foundEyes = DetectFeaturePoints(prevGray, face, eyeCascade, points,prevFrame);
	
	return foundFace && foundEyes;
}

bool BlinkManager::CheckForOutOfBoundsPoints(vector<Point2f> &points, int &rows, int &columns)
{
	int count = 0;
	int maxOutOfBoundsPoints = 8;
	for (auto &point : points)
	{
		if (point.x < 0 || point.x > columns || point.y < 0 || point.y > rows)
			count++;
	}
	bool outOfBounds = count > maxOutOfBoundsPoints ? true : false;
	return outOfBounds;
}

void BlinkManager::DisplayMessage(Mat &image, const string &message, Scalar color, int x, int y)
{
	putText(image,
		message,
		cv::Point(x, y), // Coordinates
		cv::FONT_HERSHEY_COMPLEX_SMALL, // Font
		1.0, // Scale 
		color, // Color
		1, // Thickness
		CV_AA);
}

bool BlinkManager::CheckEyeStatus(Mat &frame,Mat &processedFrame, Mat& colorFrame)
{
	//check both eyes
	blinkDetector->OpenEyeDetectedFromTemplate(frame, processedFrame,colorFrame);
	blinkDetector->OpenEyeDetectedFromTemplate(frame, processedFrame,colorFrame,false);
	//log status to console
	LogEyeStatus(blinkDetector->GetStatus());
	//handle blink
	HandleBlink(blinkDetector->UserBlinked(),colorFrame);
	return true;
}
bool BlinkManager::HandleBlink(bool blinked, Mat &frame)
{
	if (BLINK_COUNTER_MSG == BLINK_COUNTER_MSG_BASE)
		BLINK_COUNTER_MSG += blinkDetector->blinkCounter;
	if (blinked)
	{
		BLINK_COUNTER_MSG = BLINK_COUNTER_MSG_BASE + to_string(blinkDetector->blinkCounter);
	}
	DisplayMessage(frame, BLINK_COUNTER_MSG, Scalar(0, 255, 0), 250, 30);

	return true;

}
double BlinkManager::ComputeBlinksPerMinute(double duration,Mat &frame)
{
	double blinksPerMinute = blinkDetector->blinkCounter/(duration/60);
	DisplayMessage(frame, BLINKS_PER_MINUTE_MSG + to_string(blinksPerMinute), Scalar(0, 255, 0), 50, 50);
	return blinksPerMinute;
}
bool BlinkManager::LogEyeStatus(bool status)
{
	if (status)
		cout << "Eyes Are Open" << endl;
	else
		cout << "Eyes Are Closed" << endl;
	return true;
}
void BlinkManager::RunBlinkDetector() 
{
	//time variables
	bool started = false;
	std::clock_t start;

	//window to display camera capture
	namedWindow("videoCapture", 1);
	namedWindow("videoCapture2", 1);
	Point faceCenter{ 0,0 };
	
	//feature points
	vector<Point2f> prevPoints;
	vector<Point2f> points;
	
	bool foundFace = false;
	bool foundEyes = false;
	Rect face;
	Mat prevGray;

	//loop until both face and 2 eyes are detected
	while (!BasicDetection(prevGray, face,prevPoints));
	
	int min_points = 7;
	int rows = prevGray.rows;
	int columns = prevGray.cols;
	string text = "Feature Points Lost\n Please reposition yourself";

	//main loop
	for (;;)
	{
		
		if (!started)
		{
			start = std::clock();
			started = true;
		}
		
		//Check if feature points are lost
		bool outOfBounds = CheckForOutOfBoundsPoints(prevPoints, rows, columns);
		if (outOfBounds)
		{
			Scalar color(0, 255, 0);
			string m1, m2;
			m1 = "Feature Points Lost";
			m2 = "Reposition Yourself";
			Mat tempFrame;
			capture >> tempFrame;
			DisplayMessage(tempFrame, m1, color, 100, 200);
			DisplayMessage(tempFrame, m2, color, 100, 150);

			imshow("videoCapture", tempFrame);
			if (waitKey(30) >= 0) break;
			while (!BasicDetection(prevGray, face, prevPoints));
		}

		//Get new frame from buffer
		Mat frame, templateProcessedFrame;
		Mat frameGray;
		capture >> frame;

		//convert to grayscale
		cvtColor(frame, frameGray, COLOR_BGR2GRAY);
		equalizeHist(frameGray, frameGray);

		Mat resultFrame;
		frame.copyTo(resultFrame);

		//See if any blinks were detected
		CheckEyeStatus(frameGray, templateProcessedFrame,resultFrame);
		

		//Optical Flow logic
		vector<uchar> status;
		vector<float>error;
		calcOpticalFlowPyrLK(prevGray, frameGray, prevPoints, points,status,error);

		for (auto i : points) {
			line(frame,i, i, Scalar(230, 155, 255),5);
		}

		prevPoints = points;
		prevGray = frameGray;
		//optical flow logic ends

		//blink logic
		double duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
		double blinksPerMinute = ComputeBlinksPerMinute(duration,resultFrame);

		//cout << setprecision(2)<<blinksPerMinute<<endl;
		cv::imshow("videoCapture", frame);
		cv::imshow("videoCapture2", resultFrame);

		if (waitKey(30) >= 0) break;
		

	}
	
}
