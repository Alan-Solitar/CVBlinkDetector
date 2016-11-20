
#include "BlinkManager.h";
#include "HaarCascadeDetections.h"

using namespace HaarDetections;

BlinkManager::BlinkManager(const string &faceCascadeFile, const string eyeCascadeFile) {
 	SetCascades(faceCascadeFile, eyeCascadeFile);
	Init();
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

	bool foundEyes = DetectFeaturePoints(prevGray, face, eyeCascade, points);
	
	return foundFace && foundEyes;

}



bool BlinkManager::CheckForOutOfBoundsPoints(vector<Point2f> &points, int &rows, int &columns)
{
	int count = 0;
	int maxOutOfBoundsPoints = 8;
	for (auto &point : points)
	{
		cout << point.x << ", " << point.y << endl;
		if (point.x < 0 || point.x > columns || point.y < 0 || point.y > rows)
			count++;
	}
	bool outOfBounds = count > maxOutOfBoundsPoints ? true : false;
	return outOfBounds;
}

void BlinkManager::RunBlinkDetector() 
{
	
	//blink variables
	Scalar eyeColor;
	Scalar skinColor;

	//window to display camera capture
	namedWindow("videoCapture", 1);
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
	//goodFeaturesToTrack(prevGray, prevPoints, maxCorners, qualityLevel, minDistance);
	//capture >> frame;
	int min_points = 5;
	int rows = prevGray.rows;
	int columns = prevGray.cols;
	for (;;)
	{
		cout << prevPoints.size() << endl;
		
		bool outOfBounds = CheckForOutOfBoundsPoints(prevPoints, rows, columns);
		if (outOfBounds)
		{
			while (!basicDetection(prevGray, face, prevPoints));
		}
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
		rectangle(frame, points[0], points[3], Scalar(255, 0, 255), 2, 8, 0);
		

		prevPoints = points;
		prevGray = frameGray;
		
		imshow("videoCapture", frame);
		if (waitKey(30) >= 0) break;
	}
	
}
