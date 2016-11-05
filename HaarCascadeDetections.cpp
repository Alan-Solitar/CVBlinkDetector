#include<string>
#include <opencv2/core/core.hpp>
#include <opencv2\highgui.hpp>
#include <opencv2\opencv.hpp>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include"HaarCascadeDetections.h"

using namespace std;
using namespace cv;

namespace HaarDetections {

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
	void DetectEyes(CascadeClassifier eyeClassifier)
	{

	}
	vector<Point2f > DetectFeaturePoints(Mat image,Rect face)
	{
		Mat faceImageGray;
		Mat faceImage(image, face);
		imwrite("faceimage.jpg", faceImage);
		int x1, x2, x3, x4, y1, y2, y3, y4;

		x1 = face.x;
		y1 = face.y;
		x2 = x1;
		y2 = face.y + face.height;
		x3 = face.x + face.width;
		y3 = y1;
		x4 = x3;
		y4 = y2;
		
		//loop through and set all pixels not part of face to 0
		//This will make feature point detection work better
		for (int i = 0; i < image.rows; i++) {
			for (int j = 0; j < image.cols; j++) {
				//apply condition here
				if ((i>=x1) && (i <=x3) && (j>=y1 ) &&(j<=y2)) {
				}
				else {
					image.at<uchar>(i, j) = 0;
				}
			}
		}
		
		imwrite("fullimage.jpg", image);
		double qualityLevel=0.03;
		double minDistance=5;
		int maxCorners =10;
		vector<Point2f>  corners;
		corners.resize(maxCorners);
		goodFeaturesToTrack(faceImage,corners, maxCorners, qualityLevel, minDistance);
		return corners;
	}

}