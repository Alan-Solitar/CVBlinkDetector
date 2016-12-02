/*
	
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;

/// Global Variables
Mat img; Mat templ; Mat result;
char* image_window = "Source Image";
char* result_window = "Result window";

int match_method = CV_TM_SQDIFF;
int max_Trackbar = 5;

/// Function Headers
void MatchingMethod(int, void*);

int main()
{
	/// Load image and template
	img = imread("fullimage.jpg",1);
	templ = imread("eyeimage1.jpg",1);

	/// Create windows
	namedWindow(image_window, CV_WINDOW_AUTOSIZE);
	namedWindow(result_window, CV_WINDOW_AUTOSIZE);

	/// Create Trackbar
	MatchingMethod(0,0);

	waitKey(0);
	return 0;
}
void MatchingMethod(int, void*)
{
	/// Source image to display
	Mat img_display;
	img.copyTo(img_display);

	/// Create the result matrix
	int result_cols = img.cols - templ.cols + 1;
	int result_rows = img.rows - templ.rows + 1;

	result.create(result_rows, result_cols, CV_32FC1);
	match_method = CV_TM_SQDIFF;
	/// Do the Matching and Normalize
	matchTemplate(img, templ, result, match_method);
	normalize(result, result, 0, 1, NORM_MINMAX, -1, Mat());

	/// Localizing the best match with minMaxLoc
	double minVal; double maxVal; Point minLoc; Point maxLoc;
	Point matchLoc;

	minMaxLoc(result, &minVal, &maxVal, &minLoc, &maxLoc, Mat());

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
	rectangle(img_display, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);
	rectangle(result, matchLoc, Point(matchLoc.x + templ.cols, matchLoc.y + templ.rows), Scalar::all(0), 2, 8, 0);

	imshow(image_window, img_display);
	imshow(result_window, result);

	return;
}
*/

/*
*/
#include <opencv2/core/core.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\opencv.hpp>
#include "BlinkManager.h"
#include <iostream>
#include "SkinDetector.h"
using namespace cv;
using namespace std;

//short program which shows how to convert an image into greyscale
int main()
{
	const string faceCascadeFile = "cascades\\haarcascade_frontalface_default.xml";
	const string eyeCascadeFile = "cascades\\haarcascade_eye_tree_eyeglasses.xml";

	BlinkManager blinkManager(faceCascadeFile, eyeCascadeFile);
	blinkManager.RunBlinkDetector();
	VideoCapture capture;
	//open capture object at location zero (default location for webcam)

	capture.open(0);

	Mat cameraFeed;

	SkinDetector mySkinDetector;

	Mat skinMat;

	//start an infinite loop where webcam feed is copied to cameraFeed matrix
	//all of our operations will be performed within this loop
	while (1) {

		//store image to matrix
		capture >>cameraFeed;

		//show the current image
		imshow("Original Image", cameraFeed);

		skinMat = mySkinDetector.getSkin(cameraFeed);
		int TotalNumberOfPixels = skinMat.rows * skinMat.cols;
		int ZeroPixels = TotalNumberOfPixels - countNonZero(skinMat);
		cout << "The number of pixels that are zero is " << ZeroPixels << endl;
		
		imshow("Skin Image", skinMat);

		waitKey(30);
	}
	return 0;
}
