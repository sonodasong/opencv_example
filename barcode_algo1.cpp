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
	Mat out, at, sobel, sx, sy;
	vector<vector<Point> > contours;
	VideoCapture cap;
	int ratio = 0, offset = 50;
	cap.open(0);
	namedWindow("cur", WINDOW_AUTOSIZE);
	namedWindow("sobel", WINDOW_AUTOSIZE);
	namedWindow("sobel at", WINDOW_AUTOSIZE);
	namedWindow("ed", WINDOW_AUTOSIZE);
	createTrackbar("block", "sobel at", &ratio, 50, NULL);
	createTrackbar("offset", "sobel at", &offset, 100, NULL);
	while (1) {
		cap >> out;
		cvtColor(out, out, CV_BGR2GRAY);
		imshow("cur", out);
		//blur(out, out, Size(5, 5));
		xySoble(out, sobel);
		imshow("sobel", sobel);
		adaptiveThreshold(sobel, at, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 3, -offset);
		imshow("sobel at", at);
		Mat kernel = getStructuringElement(MORPH_RECT, Size(1, ratio + 1));
		morphologyEx(at, at, MORPH_OPEN, kernel);
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
/* gradient */
