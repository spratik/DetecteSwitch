// CharSegmentation.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "opencv2\opencv.hpp"
#include <iostream>
#include <vector>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include<cmath>
using namespace std;
using namespace cv;


/*
* sobel.cpp
*
*  Created on: May 26, 2018
*      Author: spratik
*/

#include <iostream>
#include <opencv2/opencv.hpp>
#include<cmath>
#include <fstream>
using namespace std;
using namespace cv;

int kernalX[3][3] = {1,0,-1,
	2,0,-2,
	1,0,-1};

int kernalY[3][3] = {1,2,1,
	0,0,0,
	-1,-2,-1};


void writePGM(cv :: Mat image, int *outputBuffer)
{
	ofstream pgmFile("Soble.pgm");
	if(!pgmFile.is_open()){
		cout << "File opening Issue" << endl;
		return;
	}

	uchar *data = image.data;

	pgmFile << "P2" << endl;
	pgmFile << image.cols << " " << image.rows  << endl;
	pgmFile << "255" << endl;



	for(int i = 0; i < image.cols * image.rows; i++){

		if(i % image.cols == 0){
			pgmFile << endl;
			pgmFile << outputBuffer[i] << " ";
		}else{
			pgmFile << outputBuffer[i] << " ";
		}
	}

	pgmFile.close();

}
//
//void getSobelEdge(Mat &image,Mat &outputImage)
//{
//
//	uchar *data = image.data;
//	int *pixelBuffer = (int *)image.data;
//	//int *pixelBuffer = new int[image.rows * image.cols];
//	int *outputBuffer = (int *)outputImage.data;
//
//	/*for(int i = 0; i < image.rows * image.cols; i++){
//		pixelBuffer[i] =  (int)data[i];
//		outputBuffer[i] =  (int)data[i];
//	}*/
//
////	cout << image.rows * image.cols << endl;
////	cout <<image.cols << endl;
////	cout << image.rows << endl;
//
//
//
//	for(int r = 1; r < image.rows - 2; r++){
//
//		for(int c = 1; c < image.cols - 2; c++){
//
//			int TL = c + r *3;
//			int TP = (c + 1) + r *image.cols;
//			int TR = (c + 2) + r *image.cols;
//
//			int ML =  c 	 + (r+1) *image.cols;
//			int MD = (c + 1) + (r+1) *image.cols;
//			int MR = (c + 2) + (r+1) *image.cols;
//
//			int BL =  c 	 + (r+2) *image.cols;
//			int BD = (c + 1) + (r+2) *image.cols;
//			int BR = (c + 2) + (r+2) *image.cols;
//
////			cout << "(" << TL << "," << TP << "," << TR << endl;
////			cout << "(" << ML << "," << MD << "," << MR << endl;
////			cout << "(" << BL << "," << BD << "," << BR << endl;
//
//			int MagX = pixelBuffer[TL] * kernalX[0][0] + pixelBuffer[TP] * kernalX[0][1] + pixelBuffer[TR] * kernalX[0][2] +
//					   pixelBuffer[ML] * kernalX[1][0] + pixelBuffer[MD] * kernalX[1][1] + pixelBuffer[MR] * kernalX[1][2] +
//				       pixelBuffer[BL] * kernalX[2][0] + pixelBuffer[BD] * kernalX[2][1] + pixelBuffer[BR] * kernalX[2][2];
//
//			int MagY = pixelBuffer[TL] * kernalY[0][0] + pixelBuffer[TP] * kernalY[0][1] + pixelBuffer[TR] * kernalY[0][2] +
//					   pixelBuffer[ML] * kernalY[1][0] + pixelBuffer[MD] * kernalY[1][1] + pixelBuffer[MR] * kernalY[1][2] +
//					   pixelBuffer[BL] * kernalY[2][0] + pixelBuffer[BD] * kernalY[2][1] + pixelBuffer[BR] * kernalY[2][2];
//
//
//			//cout << MagX << " " << MagY << endl;
//
//			unsigned int squreRoot = sqrt( (pow((double )MagX,2) + pow((double)MagY,2)));
//
//			//cout << squreRoot << endl;
//
//			if(squreRoot >= 255){
//				outputBuffer[MD] = 255;
//			}else{
//				outputBuffer[MD] = squreRoot;
//			}
//		}
//
//	}
//
//	writePGM(image,outputBuffer);
//
//	delete []pixelBuffer;
//}


//int main(int argc, char **argv) {
//
//	if(argc != 2){
//		cout << "Usgae : Sobel Input_image" << endl;
//		return 0;
//	}
//
//	Mat image = imread(argv[1],0);
//	if(!image.data){
//		cout << "Image read Error" << endl;
//		return 0;
//	}
//	getSobelEdge(image);
//
//
//
//}


void dumpDataIntoFile ( Mat image, int size ,int cols ,int rows ,char *output)

{
	ofstream myfile(output);
	/// dump P1 first into it for create the binary image

	myfile <<  "P1" << endl;

	/// dump the width and height of the image

	myfile <<  cols  <<  " " << rows << endl;

	/// scan the image for create the binary file	

	for(int i = 0; i < size ; i++ )
	{
		/// every row put one '\n' for new line for proper showing data
		if(i%cols == 0){
			myfile << endl;
		}
		/// check each index of the connected component buffer with not equal to zero if it is ture then dump one into file
		if(image.data[i]!=0){
			myfile << "1" << " ";
			/// otherwise dump zero into the file
		}else{ 
			myfile << "0" << " ";
		}
	}
	/// close the file
	myfile.close();
}

void MorphologicalOperation(Mat image, Mat &output)
{
	Mat element = getStructuringElement(cv::MORPH_RECT,cv::Size( 1 ,1 ));
	morphologyEx( image, output, MORPH_DILATE,element);
}

void MorpTOPHAT(Mat image, Mat &output)
{
	Mat element = getStructuringElement(cv::MORPH_RECT,cv::Size( 2 ,2 ));
	morphologyEx( image, output, MORPH_TOPHAT,element);
}


int _tmain(int argc, _TCHAR* argv[])
{
	//Mat image = imread("D:\\VS_workspace\\PMD_lineDetection\\Images\\IMG-20190423-WA0005.jpg");
	Mat image = imread("C:\\Users\\PRATIK\\Pictures\\BookCover.jpg");
	if(!image.data){
		cout << "read error" << endl;
		return 0;
	}
	cv::resize(image, image, cv::Size(500,700));
	Mat grey;
	GaussianBlur(image, image, Size(3, 3), 0, 0, BORDER_DEFAULT);
	//imshow("GaussianBlur",image);
	Mat grayImage;
	cv::cvtColor(image, grey, COLOR_BGR2HSV);
	cv::cvtColor(image, grayImage, COLOR_BGR2GRAY);
	Mat channel[3];
	split(grey,channel);
	Mat sat = channel[1].clone();
	Mat des ;
	Mat edge;
	Canny( sat, edge, 50, 150, 3);
	imshow("edge",edge);

	Mat element = getStructuringElement(cv::MORPH_RECT,
		cv::Size( 20 , 2));
	cout << element  << endl;
	Mat dst;
	morphologyEx( edge, dst, MORPH_DILATE, element);

	/*imshow("Dilation",dst);*/

	cv::threshold(dst,des, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	//adaptiveThreshold(sat, des, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 3, 2);
	des = 255 - des;
	/*imshow("thresholdImage",des);*/
	vector<vector<Point> > contours;
	vector<vector<Point> > filterContours;
	vector<Vec4i> hierarchy;

	findContours(des , contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0) );

	vector<Rect> boundRect( contours.size() );

	int lowerHalfCounter = 0;
	int upperHalfCounter = 0;
	Mat cropped;
	int c = 0;
	Mat croppedDest;
	Mat output;
	Mat eqHist;
	Mat topHat;
	for(int i = 0; i < contours.size(); i++)
	{
		boundRect[i] = boundingRect( contours[i]);
		//rectangle(image,boundRect[i],Scalar (0,0,255));
		bool t1 = (98 <= boundRect[i].width && boundRect[i].width <= 460);
		bool t2 = (21 <= boundRect[i].height && boundRect[i].height <=78);
		if(t1 && t2){
			cropped = sat(boundRect[i]);
			/*char buf[1024];
			sprintf(buf,"output\\image_%d_%dx%d.png",c++,croppedDest.cols,croppedDest.rows);
			imwrite(buf, cropped);*/
			//equalizeHist( cropped, eqHist );
			//cv::threshold(cropped,croppedDest, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
			adaptiveThreshold(cropped, croppedDest, 255, ADAPTIVE_THRESH_MEAN_C, THRESH_BINARY, 3, 2);
			croppedDest = 255 - croppedDest;
			imshow("MorphoInput",croppedDest);
			MorphologicalOperation(croppedDest,output);
			imshow("MorphoOutput",output);
			waitKey(0);

			findContours( output, filterContours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE, Point(0,0) );
			
			vector<Rect> segCharRect( filterContours.size() );
			/*char buf[1024];
			sprintf(buf,"output\\image_%d_%dx%d.png",c++,croppedDest.cols,croppedDest.rows);
			imwrite(buf, croppedDest);*/
			for(int j = 0; j < filterContours.size();j++){
				segCharRect[j] = boundingRect( filterContours[j]);
				if(/*(t1 && t2)*/1) {
					segCharRect [j].y += boundRect[i].y;
					segCharRect [j].x += boundRect[i].x;
					rectangle(image,segCharRect[j],Scalar (255,255,0));
				}
			}
			filterContours.clear();
		}
	}

	imshow("contoure",image);
	waitKey(0);
	return 0;
}
