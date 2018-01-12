#include "stdafx.h"

#include <opencv2\opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

void drawGradient16s(Mat &bgr, Mat &xGradient, Mat &yGradient, int x, int y)
{
	signed short xg = xGradient.at<signed short>(Point(x, y));
	signed short yg = yGradient.at<signed short>(Point(x, y));
	arrowedLine(bgr, Point(x, y), Point(x + xg, y + yg), Scalar(0, 0, 255));
}

void drawGradient8u(Mat &bgr, Mat &xGradient, Mat &yGradient, int x, int y)
{
	uchar xg = xGradient.at<uchar>(Point(x, y));
	uchar yg = yGradient.at<uchar>(Point(x, y));
	arrowedLine(bgr, Point(x, y), Point(x + xg, y + yg), Scalar(0, 0, 255));
	cout << (int)xg << " " << (int)yg << endl;
}

int main(int argc, char** argv) {
	Mat camBgr, camGray, camX, camY, camSum;

	VideoCapture cap;
	cap.open(0);

	namedWindow("camBgr", WINDOW_AUTOSIZE);
	namedWindow("camGray", WINDOW_AUTOSIZE);
	//namedWindow("camX", WINDOW_AUTOSIZE);
	//namedWindow("camY", WINDOW_AUTOSIZE);
	//namedWindow("camSum", WINDOW_AUTOSIZE);

	int x = 60, y = 60;
	createTrackbar("x", "camGray", &x, 500, NULL);
	createTrackbar("y", "camGray", &y, 500, NULL);

	//int offset = 15;
	//createTrackbar("offset", "camGray", &offset, 100, NULL);

	while (1) {
		cap >> camBgr;

		cvtColor(camBgr, camGray, COLOR_BGR2GRAY);
		//adaptiveThreshold(camGray, camGray, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, 3, -offset);

		Sobel(camGray, camX, CV_8U, 1, 0, 1);
		Sobel(camGray, camY, CV_8U, 0, 1, 1);

		//addWeighted(camX, 0.5, camY, 0.5, 0.0, camSum, CV_8U);

		drawGradient8u(camBgr, camX, camY, x, y);

		arrowedLine(camX, Point(x, y), Point(x + 10, y + 10), 255);
		arrowedLine(camY, Point(x, y), Point(x + 10, y + 10), 255);

		imshow("camBgr", camBgr);
		imshow("camGray", camGray);
		//imshow("camX", camX);
		//imshow("camY", camY);
		//imshow("camSum", camSum);

		if (waitKey(1) == 27) {
			break;
		}
	}
	destroyWindow("camBgr");
	destroyWindow("camGray");
	//destroyWindow("camX");
	//destroyWindow("camY");
	//destroyWindow("camSum");
	return 0;
}
