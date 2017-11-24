#include "stdafx.h"

#include <opencv2\opencv.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
	Mat cur, prev, out;
	vector<vector<Point> > contours;
	VideoCapture cap;
	cap.open(0);
	namedWindow("demo", WINDOW_AUTOSIZE);
	namedWindow("contour", WINDOW_AUTOSIZE);
	cap >> cur;
	while (1) {
		prev = cur.clone();
		cap >> cur;
		if (cur.empty()) {
			break;
		}
		//putText(cur, "asd", Point(60, 60), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 255, 0, 0), 1, LINE_8);
		absdiff(prev, cur, out);
		cvtColor(out, out, CV_BGR2GRAY);
		blur(out, out, Size(5, 5));
		threshold(out, out, 30, 255, CV_THRESH_BINARY);
		imshow("demo", out);
		findContours(out, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
		out = cur.clone();
		drawContours(out, contours, -1, Scalar(0, 255, 0, 0), 1, LINE_8);
		imshow("contour", out);
		if (waitKey(1) == 27) {
			break;
		}
	}
	destroyWindow("demo");
	destroyWindow("contour");
	return 0;
}
