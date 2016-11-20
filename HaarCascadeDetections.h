#pragma once
#ifndef HAAR_CASCADE_DETECTIONS_H
#define HAAR_CASCADE_DETECTIONS_H

#include<string>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\opencv.hpp>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

namespace HaarDetections {

	
	//Detection function
	Point DetectFace(CascadeClassifier faceClassifier, CascadeClassifier eyesClassifier, Mat &frame, Point currentCenter);
	bool DetectFace(CascadeClassifier faceClassifier, CascadeClassifier eyesClassifier, Mat &frame, Rect &face);
	bool DetectFeaturePoints(Mat image,Rect face, CascadeClassifier eyesClassifier, vector<Point2f> &points);
	bool DetectEyes(CascadeClassifier eyesClassifier, Mat image, Rect face, std::vector<Rect> &eyes);







}
/*
class HaarCascadeDetections
{

public:
	





};
*/
#endif // !HAAR_CASCADE_DETECTIONS_H
