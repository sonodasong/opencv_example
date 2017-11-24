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
	int ratio1 = 0, ratio2 = 0, offset = 50;
	cap.open(0);
	namedWindow("cur", WINDOW_AUTOSIZE);
	namedWindow("sobel", WINDOW_AUTOSIZE);
	namedWindow("sobel at", WINDOW_AUTOSIZE);
	namedWindow("ed", WINDOW_AUTOSIZE);
	createTrackbar("ratio 1", "sobel at", &ratio1, 50, NULL);
	createTrackbar("ratio 2", "sobel at", &ratio2, 50, NULL);
	createTrackbar("offset", "sobel at", &offset, 100, NULL);
	while (1) {
		cap >> out;
		cvtColor(out, out, CV_BGR2GRAY);
		imshow("cur", out);
		//blur(out, out, Size(5, 5));
		//xySoble(out, sobel);
		//imshow("sobel", sobel);
		adaptiveThreshold(out, at, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 3, -offset);
		imshow("sobel at", at);
		kernel = getStructuringElement(MORPH_RECT, Size(1, ratio1 + 1));
		morphologyEx(at, at, MORPH_OPEN, kernel);
		imshow("sobel", at);
		kernel = getStructuringElement(MORPH_RECT, Size(ratio2 + 1, 1));
		morphologyEx(at, at, MORPH_CLOSE, kernel);
		//erode(at, at, kernel, Point(-1, -1), 1);
		//dilate(at, at, kernel, Point(-1, -1), 1);
		//findContours(out, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
		//drawContours(cur, contours, -1, Scalar(0, 255, 0, 0), 1, LINE_8);
		imshow("ed", at);
		if (waitKey(1) == 27) {
			break;
		}
	}
	destroyWindow("cur");
	destroyWindow("soble");
	destroyWindow("soble at");
	destroyWindow("ed");
	return 0;
}

/* at, closing */
