#include "stdafx.h"

#include <opencv2\opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

void dftWrap(Mat &input, String window)
{
	Mat planes[] = { Mat_<float>(input), Mat::zeros(input.size(), CV_32F)};
	Mat complexI;
	merge(planes, 2, complexI);
	dft(complexI, complexI);
	split(complexI, planes);
	magnitude(planes[0], planes[1], planes[0]);
	Mat magI = planes[0];
	magI += Scalar::all(1);
	log(magI, magI);
	normalize(magI, magI, 0, 1, CV_MINMAX);
	imshow(window, magI);
}

int main(int argc, char** argv) {
	Mat camBgr, camGray, camAt;
	Mat kd1, ke1, kd2;

	VideoCapture cap;
	cap.open(0);

	namedWindow("camBgr", WINDOW_AUTOSIZE);
	namedWindow("camGray", WINDOW_AUTOSIZE);
	namedWindow("camGrayDft", WINDOW_AUTOSIZE);
	namedWindow("camAt", WINDOW_AUTOSIZE);
	namedWindow("camAtDft", WINDOW_AUTOSIZE);

	int blurSize = 1;
	createTrackbar("blurSize", "camGray", &blurSize, 3, NULL);

	int atOffset = 3;
	createTrackbar("atOffset", "camAt", &atOffset, 100, NULL);
	int atSize = 0;
	createTrackbar("atSize", "camAt", &atSize, 3, NULL);

	while (1) {
		cap >> camBgr;

		cvtColor(camBgr, camGray, COLOR_BGR2GRAY);
		blur(camGray, camGray, Size(blurSize * 2 + 1, blurSize * 2 + 1));


		adaptiveThreshold(camGray, camAt, 255, CV_ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY, atSize * 2 + 3, atOffset);
		camAt = Scalar::all(255) - camAt;

		dftWrap(camGray, "camGrayDft");
		dftWrap(camAt, "camAtDft");

		imshow("camBgr", camBgr);
		imshow("camGray", camGray);
		imshow("camAt", camAt);

		if (waitKey(1) == 27) {
			break;
		}
	}
	destroyWindow("camBgr");
	destroyWindow("camGray");
	destroyWindow("camGrayDft");
	destroyWindow("camAt");
	destroyWindow("camAtDft");
	return 0;
}
