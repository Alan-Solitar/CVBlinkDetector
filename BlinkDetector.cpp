#include "BlinkDetector.h"

uint BlinkDetector::blinkCounter;
bool BlinkDetector::eyeStatus;
Mat BlinkDetector::eyeOne;
Mat BlinkDetector::eyeTwo;
BlinkDetector* BlinkDetector::bt;

//0 is closed, 1 is open
BlinkDetector::BlinkDetector()
{
	BlinkDetector::blinkCounter = 0;
	BlinkDetector::eyeStatus = 1; //0 is closed, 1 is open
}
bool BlinkDetector::OpenEyeDetectedFromTemplate(Mat &image, Mat &resultImage, bool firstTemplate)
{
	Mat templateImage;
	if (firstTemplate)
		templateImage = BlinkDetector::eyeOne;
	else
		templateImage = BlinkDetector::eyeTwo;


	/// Create the result matrix
	int result_cols = image.cols - templateImage.cols + 1;
	int result_rows = image.rows - templateImage.rows + 1;
	resultImage.create(result_rows, result_cols, CV_32FC1);
	int match_method = CV_TM_SQDIFF;
	/// Do the Matching and Normalize
	matchTemplate(image, templateImage, resultImage, match_method);
	normalize(resultImage, resultImage, 0, 1, NORM_MINMAX, -1, Mat());
	/// Localizing the best match with minMaxLoc
	double minVal; double maxVal; Point minLoc; Point maxLoc;
	Point matchLoc;

	minMaxLoc(resultImage, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

	/// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
	if (match_method == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED)
	{
		matchLoc = minLoc;
	}
	else
	{
		matchLoc = maxLoc;
	}
	/// Show me what you got
	rectangle(resultImage, matchLoc, Point(matchLoc.x + templateImage.cols, matchLoc.y + templateImage.rows), Scalar::all(0), 2, 8, 0);
	return true;
}

