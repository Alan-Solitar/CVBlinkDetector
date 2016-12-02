
#include "BlinkManager.h"
#include "BlinkDetector.h"
#include "Detections.h"
#include <exception>

using namespace HaarDetections;
using namespace SkinDetections;

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
bool BlinkManager::basicDetection(Mat &prevGray, Rect &face, vector<Point2f> &points)
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
		//cout << point.x << ", " << point.y << endl;
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
		2.0, // Scale. 2.0 = 2x bigger
		color, // Color
		1, // Thickness
		CV_AA);
}

bool BlinkManager::CheckEyeStatus(Mat &frame,Mat &processedFrame)
{
	blinkDetector->OpenEyeDetectedFromTemplate(frame, processedFrame);
	blinkDetector->OpenEyeDetectedFromTemplate(frame, processedFrame,false);
	return true;
}
void BlinkManager::RunBlinkDetector() 
{
	
	//blink variables
	Scalar eyeColor;
	Scalar skinColor;

	//window to display camera capture
	namedWindow("videoCapture", 1);
	namedWindow("videoCapture2", 1);
	Point faceCenter{ 0,0 };
	//main loop
	
	//feature points
	vector<Point2f> prevPoints;
	vector<Point2f> points;
	
	bool foundFace = false;
	bool foundEyes = false;
	Rect face;
	Mat prevGray;
	//keep running loop until a face is found
	/*
	while (!foundFace || !foundEyes)
	{
		Mat prevFrame;
		capture >> prevFrame;
		foundFace = DetectFace(faceCascade, eyeCascade, prevFrame, face);

		
		cvtColor(prevFrame, prevGray, COLOR_BGR2GRAY);
		equalizeHist(prevGray, prevGray);

		foundEyes = DetectFeaturePoints(prevGray, face, eyeCascade,prevPoints);
	}
	*/
	while (!basicDetection(prevGray, face,prevPoints));
	cout << BlinkDetector::eyeOne.cols << endl;
	cout << BlinkDetector::eyeTwo.cols << endl;
	


	int min_points = 7;
	int rows = prevGray.rows;
	int columns = prevGray.cols;
	string text = "Feature Points Lost\n Please reposition yourself";

	for (;;)
	{
		//cout << prevPoints.size() << endl;
		
		bool outOfBounds = CheckForOutOfBoundsPoints(prevPoints, rows, columns);
		if (outOfBounds)
		{
			Scalar color(0, 255, 0);

			string m1, m2;
			m1 = "Feature Points Lost";
			m2 = "Reposition Yourself";
			//cout << text << endl;
			Mat tempFrame;
			capture >> tempFrame;
			DisplayMessage(tempFrame, m1, color, 100, 200);
			DisplayMessage(tempFrame, m2, color, 100, 150);

			imshow("videoCapture", tempFrame);
			if (waitKey(30) >= 0) break;
			while (!basicDetection(prevGray, face, prevPoints));
		}

		//Get new frame from buffer
		Mat frame, templateProcessedFrame;
		Mat frameGray;
		capture >> frame;

		cvtColor(frame, frameGray, COLOR_BGR2GRAY);
		equalizeHist(frameGray, frameGray);

		Mat resultFrame;
		CheckEyeStatus(frameGray, templateProcessedFrame);

		vector<uchar> status;
		vector<float>error;
		calcOpticalFlowPyrLK(prevGray, frameGray, prevPoints, points,status,error);

		//output images for testing
		imwrite("1.jpg",prevGray);
		imwrite("2.jpg", frameGray);
		
		//draw the points on the image
		for (auto i : points) {
			line(frame,i, i, Scalar(230, 155, 255),5);
		}
		cv::rectangle(frame, points[0], points[3], Scalar(255, 0, 255), 2, 8, 0);
		cv::rectangle(frame, points[4], points[6], Scalar(255, 0, 255), 2, 8, 0);
		cv::rectangle(frame, points[9], points[11], Scalar(255, 0, 255), 2, 8, 0);


		prevPoints = points;
		prevGray = frameGray;
		
		cv::imshow("videoCapture", frame);
		cv::imshow("videoCapture2", frameGray);

		if (waitKey(30) >= 0) break;
	}
	
}
