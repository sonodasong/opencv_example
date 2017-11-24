#include "stdafx.h"

#include <opencv2\opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

void xySoble(Mat &src, Mat &dst)
{
	Mat sx, sy;
	Sobel(src, sx, CV_8U, 0, 1, -1);
	Sobel(src, sy, CV_8U, 1, 0, -1);
	subtract(sy, sx, dst);
	convertScaleAbs(dst, dst);
}

int main(int argc, char** argv) {
	Mat out, at, sobel, kernel;
	vector<vector<Point> > contours;
	VideoCapture cap;
	int ratio1 = 4, ratio2 = 35, block = 0, offset = 10;
	cap.open(0);
	namedWindow("cur", WINDOW_AUTOSIZE);
	namedWindow("al", WINDOW_AUTOSIZE);
	namedWindow("close", WINDOW_AUTOSIZE);
	namedWindow("open", WINDOW_AUTOSIZE);
	createTrackbar("ratio 1", "cur", &ratio1, 50, NULL);
	createTrackbar("ratio 2", "cur", &ratio2, 50, NULL);
	createTrackbar("block", "cur", &block, 1, NULL);
	createTrackbar("offset", "cur", &offset, 100, NULL);
	while (1) {
		cap >> out;
		cvtColor(out, out, CV_BGR2GRAY);
		imshow("cur", out);
		//blur(out, out, Size(5, 5));
		//xySoble(out, sobel);
		//imshow("sobel", sobel);

		if (block == 0) {
			adaptiveThreshold(out, at, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, block * 2 + 3, -offset);
			Laplacian(at, at, CV_8U);
		}
		else {
			Laplacian(out, at, CV_8U);
			adaptiveThreshold(at, at, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 3, -offset);
		}
		imshow("al", at);
		
		//vector<Vec4i> lines;
		//HoughLinesP(at, lines, 1, CV_PI / 180, 50, ratio2 + 1, ratio1 + 1);
		/*
		for (size_t i = 0; i < lines.size(); i++)
		{
			Vec4i l = lines[i];
			line(out, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(0, 0, 255), 3, CV_AA);
		}
		*/

		kernel = getStructuringElement(MORPH_RECT, Size(ratio1 + 1, ratio1 + 1));
		morphologyEx(at, at, MORPH_CLOSE, kernel);
		imshow("close", at);
		kernel = getStructuringElement(MORPH_RECT, Size(ratio2 + 1, ratio2 + 1));
		morphologyEx(at, at, MORPH_OPEN, kernel);
		//erode(at, at, kernel, Point(-1, -1), 1);
		//dilate(at, at, kernel, Point(-1, -1), 1);
		//findContours(out, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
		//drawContours(cur, contours, -1, Scalar(0, 255, 0, 0), 1, LINE_8);
		imshow("open", at);
		if (waitKey(1) == 27) {
			break;
		}
	}
	destroyWindow("cur");
	destroyWindow("al");
	destroyWindow("close");
	destroyWindow("open");
	return 0;
}

/* adaptive, laplacian, morphology */
