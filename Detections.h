#pragma once
#ifndef DETECTIONS_H
#define DETECTIONS_H

#include<string>
#include <vector>
#include <opencv2/core/core.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\opencv.hpp>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;

namespace HaarDetections
{
	//Detection function using Haar Cascades
	Point DetectFace(CascadeClassifier faceClassifier, CascadeClassifier eyesClassifier, Mat &frame, Point currentCenter);
	bool DetectFace(CascadeClassifier faceClassifier, CascadeClassifier eyesClassifier, Mat &frame, Rect &face);
	bool DetectFeaturePoints(Mat image,Rect face, CascadeClassifier eyesClassifier, vector<Point2f> &points, Mat prevFrame);
	bool DetectEyes(CascadeClassifier eyesClassifier, Mat image, Rect face, std::vector<Rect> &eyes, Mat prevFrame);
}

namespace SkinDetections
{
	const Mat DetectSkin(const Mat &image);
	
}
namespace TemplateMatchings
{
	
}

#endif // !HAAR_CASCADE_DETECTIONS_H
