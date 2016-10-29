
#include "BlinkManager.h";

BlinkManager::BlinkManager(const string &faceCascadeFile, const string eyeCascadeFile) {
	SetCascades(faceCascadeFile, eyeCascadeFile);
}
bool BlinkManager::SetCascades(const string &faceCascadeFile, const string eyeCascadeFile) {
	faceCascade.load(faceCascadeFile);
	eyeCascade.load(eyeCascadeFile);

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
}