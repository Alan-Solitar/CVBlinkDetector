
#include "BlinkManager.h";
#include "HaarCascadeDetections.h"

using namespace HaarDetections;

BlinkManager::BlinkManager(const string &faceCascadeFile, const string eyeCascadeFile) {
 	SetCascades(faceCascadeFile, eyeCascadeFile);
	Init();
}
bool BlinkManager::SetCascades(const string &faceCascadeFile, const string eyeCascadeFile) {
	
	//load face cascade
	if (!faceCascade.load(faceCascadeFile))
		return false;

	//load eye cascade
	if (!eyeCascade.load(eyeCascadeFile))
		return false;
	return true;
}

bool BlinkManager::Init() {
	bool allGood = true;
	allGood = InitCamera();
	return allGood;
}
bool BlinkManager::InitCamera() {
	capture = VideoCapture(0);
	return capture.isOpened();
}
void BlinkManager::RunBlinkDetector() {
	
	//window to display camera capture
	namedWindow("videoCapture", 1);
	Point faceCenter{ 0,0 };
	//namedWindow("blur", 1);
	//main loop
	Mat prevFrame;
	
	vector<Point2f> prevPoints;
	vector<Point2f> points;
	
	capture >> prevFrame;
	Mat prevGray;
	cvtColor(prevFrame, prevGray, COLOR_BGR2GRAY);
	Rect face = DetectFace(faceCascade, eyeCascade, prevFrame);
	vector<Point2f>  corners;
	corners = DetectFeaturePoints(prevGray, face);
	double qualityLevel = 0.01;
	double minDistance = 30;
	int maxCorners = 30;
	goodFeaturesToTrack(prevGray, prevPoints, maxCorners, qualityLevel, minDistance);
	//capture >> frame;
	for (;;)
	{
		//Get new frame from buffer
		Mat frame;
		Mat frameGray;
		capture >> frame;
		cvtColor(prevFrame, frameGray, COLOR_BGR2GRAY);
		vector<uchar> status;
		vector<float>error;
		calcOpticalFlowPyrLK(prevGray, frameGray, prevPoints, points,status,error);
		//prevGray = frameGray;
		//corners.resize(maxCorners);
		//Rect face =DetectFace(faceCascade, eyeCascade, frame);
		//prevPoints = DetectFeaturePoints(frame, face);
		
		
		//for (int i = 0; i < points.size(); i++) {
		//	cv::circle(prevFrame, points[i], 10, cv::Scalar(255.), -1);
		//}
		//GaussianBlur(frame, blurred, Size(9, 9), 1.5, 1.5);
		//faceCenter = DetectFace(faceCascade, eyeCascade, frame,faceCenter);
		//imshow("videoCapture", prevFrame);
		//if (waitKey(30) >= 0) break;
	}
}
