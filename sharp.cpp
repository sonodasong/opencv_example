#include "stdafx.h"

#include <opencv2\opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
	Mat camBgr, camGray, camBlur, camSharp;

	VideoCapture cap;
	cap.open(0);

	namedWindow("camBgr", WINDOW_AUTOSIZE);
	namedWindow("camGray", WINDOW_AUTOSIZE);
	namedWindow("camBlur", WINDOW_AUTOSIZE);
	namedWindow("camSharp", WINDOW_AUTOSIZE);

	while (1) {
		cap >> camBgr;

		cvtColor(camBgr, camGray, COLOR_BGR2GRAY);

		GaussianBlur(camGray, camBlur, Size(0, 0), 3);
		addWeighted(camGray, 1.5, camBlur, -0.5, 0, camSharp);

		imshow("camBgr", camBgr);
		imshow("camGray", camGray);
		imshow("camBlur", camBlur);
		imshow("camSharp", camSharp);

		if (waitKey(1) == 27) {
			break;
		}
	}
	destroyWindow("camBgr");
	destroyWindow("camGray");
	destroyWindow("camBlur");
	destroyWindow("camSharp");
	return 0;
}
