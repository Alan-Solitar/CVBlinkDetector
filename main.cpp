#include <opencv2/core/core.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\opencv.hpp>
#include "BlinkManager.h"
#include <iostream>
using namespace cv;
using namespace std;


//short program which shows how to convert an image into greyscale
int main()
{
	const string faceCascadeFile = "cascades\\haarcascade_frontalface_default.xml";
	const string eyeCascadeFile = "cascades\\haarcascade_eye_tree_eyeglasses.xml";

	BlinkManager blinkManager(faceCascadeFile, eyeCascadeFile);
	blinkManager.RunBlinkDetector();

}