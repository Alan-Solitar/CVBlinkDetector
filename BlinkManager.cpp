
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
	for (;;)
	{
		//Get new frame from buffer
		Mat frame;
		capture >> frame;
		Mat blurred;
		//some smoothing
		GaussianBlur(frame, blurred, Size(9, 9), 1.5, 1.5);
		faceCenter = DetectFace(faceCascade, eyeCascade, frame,faceCenter);
		imshow("videoCapture", frame);
		//imshow("blur", blurred);
		if (waitKey(30) >= 0) break;
	}
}
