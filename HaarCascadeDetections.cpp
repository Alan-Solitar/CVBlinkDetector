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

	Point DetectFace(CascadeClassifier faceClassifier, CascadeClassifier eyesClassifier, Mat &frame, Point currentCenter)
	{
		int targetWidth = 6;
		int targetHeight = 6;
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

			//if (eyes.size() > 0) {
				Point newCenter(face.x + face.width / 2, face.y + face.height / 2);
				
				if (currentCenter.x==0 && currentCenter.y==0) 
				{
					currentCenter = newCenter;
				}

				else 
				{

				}
				
				
		//	} 
			
			//draw ellipse
			 ellipse(frame, currentCenter, faceSize, 0, 0, 360, Scalar(255, 0, 255), 4, 8, 0);
			//draw rectangle
			 
			//  rectangle(frame,  Rect(Point(face.x, face.y), Point(face.x + face.width, face.y + face.height)) , cvScalar(255, 255, 255));
			/*
			for (size_t j = 0; j < eyes.size(); j++)
			{
				Point eye_center(faces[i].x + eyes[j].x + eyes[j].width / 2, faces[i].y + eyes[j].y + eyes[j].height / 2);
				int radius = cvRound((eyes[j].width + eyes[j].height)*0.25);
				circle(frame, eye_center, radius, Scalar(255, 0, 0), 4, 8, 0);
			}
			*/
		}
		return currentCenter;
	}
	void DetectEyes(CascadeClassifier eyeClassifier)
	{

	}

}