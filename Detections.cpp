//Written by Alan Solitar

#include<string>
#include <opencv2/core/core.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\opencv.hpp>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include"Detections.h"
#include "BlinkDetector.h"

using namespace std;
using namespace cv;
namespace HaarDetections 
{

	bool DetectFace(CascadeClassifier faceClassifier, CascadeClassifier eyesClassifier, Mat &frame, Rect &face)
	{

		std::vector<Rect> faces;
		Mat frame_gray;
		Size currentSize;
		//image preparation for face detection
		cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
		equalizeHist(frame_gray, frame_gray);

		//-- Detect faces
		faceClassifier.detectMultiScale(frame_gray, faces, 1.1, 3, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

		if (faces.size() <= 0)
			return false;
		for (int i = 0; i < faces.size(); i++)
		{
			if (faces[i].area() > face.area())
				face = faces[i];
		}
		return true;

	}
	bool DetectEyes(CascadeClassifier eyesClassifier,Mat image, Rect face, std::vector<Rect> &eyes, Mat prevFrame)
	{
		eyes.clear();
		eyesClassifier.detectMultiScale(image, eyes, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

		//only continue if exactly 2 eyes were found
		if (eyes.size() != 2)
			return false;
		int counter = 0;
		for (auto eye : eyes)
		{
			Mat eyeImage(image, eye);
			Mat eyeImage2(prevFrame, eye);
			counter++;
			if (counter == 1)
				BlinkDetector::eyeOne = eyeImage; //template for eye one
			else
				BlinkDetector::eyeTwo = eyeImage; //template for eye two
			imwrite(("eyeimage" + to_string(counter) + ".jpg"), eyeImage);
			counter++;
		}
		return true;
	}
	
	bool DetectFeaturePoints(Mat image,Rect face, CascadeClassifier eyesClassifier, vector<Point2f> &points, Mat prevFrame)
	{
		points.clear();
		vector<Rect> eyes;
		bool foundEyes = DetectEyes(eyesClassifier, image, face, eyes,prevFrame);
		if (!foundEyes)
			return false;
		Mat faceImageGray;
		Mat faceImage(image, face);
		imwrite("faceimage.jpg", faceImage);
		int x1, x2, x3, x4, y1, y2, y3, y4;

		//get values for points to be tracked
		x1 = face.x;
		y1 = face.y;
		x2 = x1;
		y2 = face.y + face.height;
		x3 = face.x + face.width;
		y3 = y1;
		x4 = x3;
		y4 = y2;
		
		imwrite("fullimage.jpg", image);
		double qualityLevel=0.02;
		double minDistance=10;
		int maxpoints =30;
		
		/* Points for face
		points.push_back(Point2f(x1, y1));
		points.push_back(Point2f(x2, y2));
		points.push_back(Point2f(x3, y3));
		points.push_back(Point2f(x4, y4));
		*/

		//calculate points on eyes
		for (int i = 0; i < eyes.size(); i++)
		{
			auto eye = eyes[i];
			Point2f topLeftPt(eye.x, eye.y);
			Point2f topRightPt(eye.x+eye.width, eye.y);
			Point2f bottomRightPt(eye.x + eye.width, eye.y + eye.height);
			Point2f bottomLeftPt(eye.x, eye.y+eye.width);
			Point2f centerPt((topLeftPt.x + topRightPt.x) / 2, (topLeftPt.y + bottomLeftPt.y) / 2);

			
			//push points into vector
			points.push_back(topLeftPt);
			points.push_back(topRightPt);
			points.push_back(bottomRightPt);
			points.push_back(bottomLeftPt);
			points.push_back(centerPt);


		}
		return true;
	}

}

/*
//not using this right now
namespace SkinDetections
{
	const Mat DetectSkin(const Mat &image)
	{
		SkinDetector mySkinDetector;

		Mat skinMat;
		skinMat = mySkinDetector.getSkin(image);
		int TotalNumberOfPixels = skinMat.rows * skinMat.cols;
		int ZeroPixels = TotalNumberOfPixels - countNonZero(skinMat);
		imshow("Skin Image", skinMat);
		return skinMat;
	}
}
*/