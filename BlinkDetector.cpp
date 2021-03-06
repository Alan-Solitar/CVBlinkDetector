//Written by Alan Solitar

#include "BlinkDetector.h"

uint BlinkDetector::blinkCounter;
pair<bool,bool> BlinkDetector::previousEyeStatus;
pair<bool, bool> BlinkDetector::currentEyeStatus;

Mat BlinkDetector::eyeOne;
Mat BlinkDetector::eyeTwo;
BlinkDetector* BlinkDetector::bt;

BlinkDetector::BlinkDetector()
{
	BlinkDetector::blinkCounter = 0;
	BlinkDetector::previousEyeStatus = make_pair(true,true); //0 is closed, 1 is open
	BlinkDetector::currentEyeStatus = make_pair(true, true); //0 is closed, 1 is open
	prevLocation = make_pair(Point(0, 0),Point(0,0));
}
bool BlinkDetector::OpenEyeDetectedFromTemplate(Mat &image, Mat &resultImage, Mat& colorFrame,bool firstTemplate)
{
	bool eyeStatus;
	Mat templateImage;
	if (firstTemplate)
		templateImage = BlinkDetector::eyeOne;
	else
		templateImage = BlinkDetector::eyeTwo;

	// Create the result matrix
	int result_cols = image.cols - templateImage.cols + 1;
	int result_rows = image.rows - templateImage.rows + 1;
	resultImage.create(result_rows, result_cols, CV_32FC1);
	int match_method = TM_CCOEFF_NORMED;
	// Do the Matching and Normalize
	matchTemplate(image, templateImage, resultImage, match_method);
	//normalize(resultImage, resultImage, 0, 1, NORM_MINMAX, -1, Mat());
	// Localizing the best match with minMaxLoc
	double minVal; double maxVal; Point minLoc; Point maxLoc;
	Point matchLoc;

	minMaxLoc(resultImage, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

	// For SQDIFF and SQDIFF_NORMED, the best matches are lower values. For all the other methods, the higher the better
	// Leaving this logic in here so I can continue to test other match methods
	if (match_method == CV_TM_SQDIFF || match_method == CV_TM_SQDIFF_NORMED)
	{
		matchLoc = minLoc;
	}
	else
	{
		matchLoc = maxLoc;
	}
	
	//Find out status of eyes
	Point prevEyeLocation;
	Rect rec(matchLoc, Point(matchLoc.x + templateImage.cols, matchLoc.y + templateImage.rows));

	//set locations of center of eyes
	if (firstTemplate)
		prevEyeLocation = prevLocation.first;
	else
		prevEyeLocation = prevLocation.second;

	if (prevEyeLocation.x == 0 & prevEyeLocation.y == 0)
	{
		prevEyeLocation = Point(rec.x + rec.width / 2, rec.y + rec.height / 2);
	}

	//see if rectangle contains previous location of eye
	if (rec.contains(prevEyeLocation))
	{
		if (firstTemplate)
		{
			prevLocation.first = Point(rec.x + rec.width / 2, rec.y + rec.height / 2);
			currentEyeStatus.first = eyeStatus = true;
		}
		else
		{
			prevLocation.second = Point(rec.x + rec.width / 2, rec.y + rec.height / 2);
			currentEyeStatus.second = eyeStatus = true;
		}
	}
	else 
	{	
		if (firstTemplate)
			currentEyeStatus.first =eyeStatus= false;
		else
			currentEyeStatus.second =eyeStatus = false;
	}
	//draw point for center of eye
	if(firstTemplate)
		line(image, prevLocation.first, prevLocation.first, Scalar(230, 155, 255), 5);
	else
		line(image, prevLocation.second, prevLocation.second, Scalar(230, 155, 255), 5);

	if(eyeStatus)
		rectangle(colorFrame, matchLoc, Point(matchLoc.x + templateImage.cols, matchLoc.y + templateImage.rows), Scalar::all(0), 2, 8, 0);
	rectangle(resultImage, matchLoc, Point(matchLoc.x + templateImage.cols, matchLoc.y + templateImage.rows), Scalar::all(0), 2, 8, 0);
	return true;
}
bool BlinkDetector::UserBlinked()
{
	bool blinked = false;
	if((previousEyeStatus.first&& !currentEyeStatus.first)||
		(previousEyeStatus.second && !currentEyeStatus.second))
	{
		blinked = true;
		blinkCounter++;
		
	}
	previousEyeStatus =currentEyeStatus;
	return blinked;
}
bool BlinkDetector::GetStatus()
{
	
	return currentEyeStatus.first && currentEyeStatus.second;
}
