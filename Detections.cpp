#include<string>
#include <opencv2/core/core.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\opencv.hpp>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include"Detections.h"
#include "SkinDetector.h"
#include "BlinkDetector.h"

using namespace std;
using namespace cv;
using namespace SkinDetections;
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
	Point DetectFace(CascadeClassifier faceClassifier, CascadeClassifier eyesClassifier, Mat &frame, Point currentCenter)
	{
		int targetWidth = 8;
		int targetHeight = 8;
		std::vector<Rect> faces;
		Mat frame_gray;
		Size currentSize;

		//image preparation for face detection
		cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
		equalizeHist(frame_gray, frame_gray);

		//-- Detect faces
		faceClassifier.detectMultiScale(frame_gray, faces, 1.1, 3, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
		for (size_t i = 0; i < faces.size(); i++)
		{
			Mat faceROI = frame_gray(faces[i]);
			Rect face = faces[i];
			
			//This will skip faces that are too small
			if (face.width < frame.size().width / targetWidth &&
				face.height < frame.size().height / targetHeight)
				continue;

			std::vector<Rect> eyes;
			Size faceSize = Size(face.width / 2, face.height / 2);

			//-- In each face, detect eyes
			eyesClassifier.detectMultiScale(faceROI, eyes, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

			if (eyes.size() > 0) {
				Point newCenter(face.x + face.width / 2, face.y + face.height / 2);
				
				if (currentCenter.x==0 && currentCenter.y==0) 
				{
					currentCenter = newCenter;
				}
				else 
				{
					if (abs(currentCenter.x - newCenter.x) < 7 &&
						abs(currentCenter.y - newCenter.y) < 7)
					{
						currentCenter = newCenter;
					}
					// Smooth new center compared to old center
					newCenter.x = (newCenter.x + 2 * currentCenter.x) / 3;
					newCenter.y = (newCenter.y + 2 * currentCenter.y) / 3;
					currentCenter = newCenter;
				}
			} 
			//draw ellipse
			 ellipse(frame, currentCenter, faceSize, 0, 0, 360, Scalar(255, 0, 255), 4, 8, 0);
			//draw rectangle
			 
			//  rectangle(frame,  Rect(Point(face.x, face.y), Point(face.x + face.width, face.y + face.height)) , cvScalar(255, 255, 255));
			
			for (size_t j = 0; j < eyes.size(); j++)
			{
				Point eye_center(faces[i].x + eyes[j].x + eyes[j].width / 2, faces[i].y + eyes[j].y + eyes[j].height / 2);
				int radius = cvRound((eyes[j].width + eyes[j].height)*0.25);
				circle(frame, eye_center, radius, Scalar(255, 0, 0), 4, 8, 0);
			}
			
		}
		return currentCenter;
	}
	bool DetectEyes(CascadeClassifier eyesClassifier,Mat image, Rect face, std::vector<Rect> &eyes, Mat prevFrame)
	{
		eyes.clear();
		eyesClassifier.detectMultiScale(image, eyes, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));
		if (eyes.size() != 2)
			return false;
		int counter = 0;
		for (auto eye : eyes)
		{
			Mat eyeImage(image, eye);
			Mat eyeImage2(prevFrame, eye);
			counter++;
			if (counter == 1)
				BlinkDetector::eyeOne = eyeImage;
			else
				BlinkDetector::eyeTwo = eyeImage;
			imwrite(("eyeimage" + to_string(counter) + ".jpg"), eyeImage);
			//DetectSkin(eyeimage2);
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
		points.push_back(Point2f(x1, y1));
		points.push_back(Point2f(x2, y2));
		points.push_back(Point2f(x3, y3));
		points.push_back(Point2f(x4, y4));

		//calculate points on eyes
		for (int i = 0; i < eyes.size(); i++)
		{
			auto eye = eyes[i];
			Point2f topLeftPt(eye.x, eye.y);
			Point2f topRightPt(eye.x+eye.width, eye.y);
			Point2f bottomRightPt(eye.x + eye.width, eye.y + eye.height);
			Point2f bottomLeftPt(eye.x, eye.y+eye.width);
			Point2f centerPt((topLeftPt.x + topRightPt.x) / 2, (topLeftPt.y + bottomLeftPt.y) / 2);
			//Point2f centerLeftPt(topLeftPt.x, (topLeftPt.y + bottomLeftPt.y) / 2);
			//Point2f centerRightPt(topRightPt.x, (topLeftPt.y + bottomLeftPt.y) / 2);

			
			//push points into vector
			points.push_back(topLeftPt);
			points.push_back(topRightPt);
			points.push_back(bottomRightPt);
			points.push_back(bottomLeftPt);
			points.push_back(centerPt);
			//points.push_back(centerLeftPt);
			//points.push_back(centerRightPt);

			for (auto pt : points)
			{
				//cout << pt.x << " " <<pt.y<<" "<<image.at<Vec3b>(pt.x,pt.y) << endl;
			}
			//rectangle(image, pt1, pt2, Scalar(255, 0, 255), 2, 8, 0);

		}
		//goodFeaturesToTrack(image,points, maxpoints, qualityLevel, minDistance);
		
		return true;
	}

}
namespace SkinDetections
{
	const Mat DetectSkin(const Mat &image)
	{
		SkinDetector mySkinDetector;

		Mat skinMat;
		skinMat = mySkinDetector.getSkin(image);
		int TotalNumberOfPixels = skinMat.rows * skinMat.cols;
		int ZeroPixels = TotalNumberOfPixels - countNonZero(skinMat);
		cout << "The number of pixels that are zero is " << ZeroPixels << endl;
		imshow("Skin Image", skinMat);
		return skinMat;
	}
}