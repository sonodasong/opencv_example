#include "stdafx.h"

#include <opencv2\opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

void show_histogram(std::string const& name, cv::Mat1b const& image)
{
	// Set histogram bins count
	int bins = 256;
	int histSize[] = { bins };
	// Set ranges for histogram bins
	float lranges[] = { 0, 256 };
	const float* ranges[] = { lranges };
	// create matrix for histogram
	cv::Mat hist;
	int channels[] = { 0 };

	// create matrix for histogram visualization
	int const hist_height = 256;
	cv::Mat3b hist_image = cv::Mat3b::zeros(hist_height, bins);

	cv::calcHist(&image, 1, channels, cv::Mat(), hist, 1, histSize, ranges, true, false);

	double max_val = 0;
	minMaxLoc(hist, 0, &max_val);

	// visualize each bin
	for (int b = 0; b < bins; b++) {
		float const binVal = hist.at<float>(b);
		int   const height = cvRound(binVal*hist_height / max_val);
		cv::line
		(hist_image
			, cv::Point(b, hist_height - height), cv::Point(b, hist_height)
			, cv::Scalar::all(255)
		);
	}
	cv::imshow(name, hist_image);
}

int main(int argc, char** argv) {
	Mat camBgr, camGray, camAt;
	Mat kd1, ke1, kd2;

	VideoCapture cap;
	cap.open(0);

	namedWindow("camBgr", WINDOW_AUTOSIZE);
	namedWindow("camGray", WINDOW_AUTOSIZE);
	namedWindow("camAt", WINDOW_AUTOSIZE);
	namedWindow("camHis", WINDOW_AUTOSIZE);

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

		show_histogram("camHis", camGray);

		imshow("camBgr", camBgr);
		imshow("camGray", camGray);
		imshow("camAt", camAt);

		if (waitKey(1) == 27) {
			break;
		}
	}
	destroyWindow("camBgr");
	destroyWindow("camGray");
	destroyWindow("camAt");
	destroyWindow("camHis");
	return 0;
}
