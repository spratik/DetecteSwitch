// SwitchDetection.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "opencv2\opencv.hpp"
#include <iostream>
#include <vector>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
using namespace std;
using namespace cv;
int _tmain(int argc, _TCHAR* argv[])
{
	Mat image = imread("D:\\VS_workspace\\PMD_lineDetection\\Images\\IMG-20190423-WA0005.jpg");
	if(!image.data){
		cout << "read error" << endl;
		return 0;
	}
	Mat grey;
	cv::cvtColor(image, grey, CV_BGR2GRAY);
	Mat des = grey.clone();
	/*unsigned char * data = static_cast<unsigned char *>(image.data);
	unsigned char * thresholdImage = static_cast<unsigned char *>(des.data);

	for(int i = 0; i < image.rows * image.cols;i++){

	if(data[i] <= 80){
	thresholdImage[i] = 255;	
	}else{
	thresholdImage[i] = 0;
	}
	}*/

	cv::threshold(grey, des, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	des = 255 - des;
	imshow("original",grey);
	imshow("thresholdImage",des);

	int erosion_size = 2;  
	Mat element = getStructuringElement(cv::MORPH_RECT,
		cv::Size( erosion_size , erosion_size));
	//cout << element  << endl;
	Mat dst;
	morphologyEx( des, dst, MORPH_OPEN, element);
	erosion_size = 1;  
	Mat element1 = getStructuringElement(cv::MORPH_RECT,
		cv::Size( erosion_size , erosion_size));
	morphologyEx( dst, dst, MORPH_CLOSE, element1);
	erosion_size = 5;  
	Mat element2 = getStructuringElement(cv::MORPH_RECT,
		cv::Size( erosion_size , erosion_size));
	morphologyEx( dst, dst, MORPH_ERODE, element2);
	erosion_size = 2;  
	Mat element3 = getStructuringElement(cv::MORPH_RECT,
		cv::Size( erosion_size , erosion_size));
	morphologyEx( dst, dst, MORPH_DILATE, element3);

	///// Apply erosion or dilation on the image
	//erode(dst,dst,element1);  // dilate(image,dst,element);
	imshow("erosion",dst);
	vector<vector<Point> > contours;
	vector<vector<Point> > filterContours;
	vector<Vec4i> hierarchy;

	findContours( dst, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );

	//for(unsigned int i = 0, j = 0; i < hierarchy.size();i++)
	//{
	//	if(/*hierarchy[i].val[2] != -1 &&*/ hierarchy[i].val[3] != -1){
	//		filterContours.push_back(contours[i]);
	//	}
	//}
	vector<Rect> boundRect( contours.size() );

	int lowerHalfCounter = 0;
	int upperHalfCounter = 0;
	Mat cropped;
	int c = 0;
	for(int i = 0; i < contours.size(); i++)
	{
		boundRect[i] = boundingRect( contours[i]);
		

		int area  = boundRect[i].width *  boundRect[i].height;
		bool t1 = (15 <= boundRect[i].width && boundRect[i].width <= 25);
		bool t2 = (13 <= boundRect[i].height && boundRect[i].height <= 25);		

		if(/*area > 100 && area < 700*/t1 && t2){
			
			cropped = dst(boundRect[i]);
			findContours( cropped, filterContours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE, Point(0, 0) );
			if(filterContours.size() == 1){
				/*char buf[1024];
				sprintf(buf,"output\\image_%d_%dx%d.png",c++,cropped.cols,cropped.rows);
				imwrite(buf, cropped);
				filterContours.clear();*/
				int row = boundRect[i].y ;
				int col = boundRect[i].x ;
				boundRect[i].height += 3;
				boundRect[i].width += 3;

				int UpperHalfStartRow = row;
				int UpperHalfStartCol = col;
				int upperHalfEndRow = row + boundRect[i].height/2;
				int upperHalfEndCol = col + boundRect[i].width;

				int LowerHalfStartRow = row + boundRect[i].height/2;
				int LowerHalfStartCol = col ;
				int LowerHalfEndRow = LowerHalfStartRow + boundRect[i].height/2+1;
				int LowerHalfEndCol = LowerHalfStartCol + boundRect[i].width;

				// UpperHalf WhitePixle Calculation
				for(int r = UpperHalfStartRow; r <upperHalfEndRow ; r++){

					for(int c = UpperHalfStartCol; c <upperHalfEndCol; c++){

						int index = r * image.cols + c;

						if(dst.data[index] == 255){
							upperHalfCounter++;
						}

					}
				}

				// LowerHalf WhitePixle Calculation
				for(int r = LowerHalfStartRow; r < LowerHalfEndRow; r++){

					for(int c = LowerHalfStartCol; c <LowerHalfEndCol; c++){

						int index = r * image.cols + c;

						if(dst.data[index] == 255){
							lowerHalfCounter++;
						}

					}
				}

				//line(dst, Point(LowerHalfStartCol,LowerHalfStartRow), Point(LowerHalfEndCol, LowerHalfEndRow), Scalar(255,0,0));
				//line(dst, Point(UpperHalfStartCol, UpperHalfStartRow), Point(upperHalfEndCol, upperHalfEndRow), Scalar(255,0,0));
				//line(image, Point(UpperHalfStartCol, upperHalfEndRow), Point(upperHalfEndCol, upperHalfEndRow), Scalar(0,255,0));

				if(upperHalfCounter < lowerHalfCounter){
					
					putText(image, "OFF", Point(UpperHalfStartCol,UpperHalfStartRow), FONT_HERSHEY_PLAIN, 0.9, Scalar(0,0,255), 1);
					rectangle(image,boundRect[i],Scalar (255,255,0));
				}else{
					putText(image, "ON", Point(UpperHalfStartCol,UpperHalfStartRow), FONT_HERSHEY_PLAIN, 0.9, Scalar(255,0,255), 1);
					rectangle(image,boundRect[i],Scalar (0,255,0));
				}
				upperHalfCounter = 0;
				lowerHalfCounter = 0;
			}else{
				/*if(filterContours.size() < 2){
					for(int  j = 0; j < filterContours.size(); j++){
						Rect rect = boundingRect( filterContours[j]);
						rect.x += boundRect[i].x;
						rect.y += boundRect[i].y;
						rect.width += 5;
						rect.height += 5;
						rectangle(image,rect,Scalar (255,0,255));
					}
				}*/
			}
		}

	}

	imshow("contoure",image);
	waitKey(0);
	return 0;
}
