//Written by Alan Solitar

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
	//Detection face using Haar Cascades
	bool DetectFace(CascadeClassifier faceClassifier, CascadeClassifier eyesClassifier, Mat &frame, Rect &face);
	//DetectFeaturePoints to be tracked by optical flow
	bool DetectFeaturePoints(Mat image,Rect face, CascadeClassifier eyesClassifier, vector<Point2f> &points, Mat prevFrame);
	//Detect eyes using Haar Cascades 
	bool DetectEyes(CascadeClassifier eyesClassifier, Mat image, Rect face, std::vector<Rect> &eyes, Mat prevFrame);
}

/*
//not using this right now
namespace SkinDetections
{
	const Mat DetectSkin(const Mat &image);
}
*/

#endif // !HAAR_CASCADE_DETECTIONS_H
