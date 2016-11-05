
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
	//main loop
	Mat prevFrame;
	
	//feature points
	vector<Point2f> prevPoints;
	vector<Point2f> points;
	
	bool foundFace = false;
	Rect face;

	//keep running loop until a face is found
	while (!foundFace)
	{
		capture >> prevFrame;
		foundFace = DetectFace(faceCascade, eyeCascade, prevFrame,face);
	}
	

	Mat prevGray;
	cvtColor(prevFrame, prevGray, COLOR_BGR2GRAY);
	equalizeHist(prevGray, prevGray);

	prevPoints= DetectFeaturePoints(prevGray, face,eyeCascade);
	//goodFeaturesToTrack(prevGray, prevPoints, maxCorners, qualityLevel, minDistance);
	//capture >> frame;
	
	for (;;)
	{
		//Get new frame from buffer
		Mat frame;
		Mat frameGray;
		capture >> frame;
		cvtColor(frame, frameGray, COLOR_BGR2GRAY);
		equalizeHist(frameGray, frameGray);
		vector<uchar> status;
		vector<float>error;
		calcOpticalFlowPyrLK(prevGray, frameGray, prevPoints, points,status,error);
		imwrite("1.jpg",prevGray);
		imwrite("2.jpg", frameGray);
		
		for (auto i : points) {
			line(frame,i, i, Scalar(230, 155, 255),5);
		}
		//rectangle(frame, points[0], points[3], Scalar(255, 0, 255), 2, 8, 0);
		

		prevPoints = points;
		prevGray = frameGray;
		
		imshow("videoCapture", frame);
		if (waitKey(30) >= 0) break;
	}
	
}
