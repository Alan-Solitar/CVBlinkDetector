
//SkinDetector.cpp

#include "SkinDetector.h"
#include"opencv2\opencv.hpp"

SkinDetector::SkinDetector(void)
{
	//YCrCb threshold
	Y_MIN = 0;
	Y_MAX = 255;
	Cr_MIN = 133;
	Cr_MAX = 173;
	Cb_MIN = 77;
	Cb_MAX = 127;
}


//this function will return a skin masked image
cv::Mat SkinDetector::getSkin(cv::Mat input)
{
	cv::Mat skin;
	//first convert our RGB image to YCrCb
 	cv::cvtColor(input, skin, cv::COLOR_BGR2YCrCb);

	//filters the image in YCrCb color space
	cv::inRange(skin, cv::Scalar(Y_MIN, Cr_MIN, Cb_MIN), cv::Scalar(Y_MAX, Cr_MAX, Cb_MAX), skin);

	return skin;
}
