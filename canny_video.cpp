#include "stdafx.h"

#include "opencv\cv.h"
#include "opencv\highgui.h"
#include <iostream>

using namespace cv;
using namespace std;

CvCapture *capture = NULL;
IplImage* frame;
IplImage* out;
int position = 0;
int low = 10;
int high = 30;

IplImage* smooth(IplImage* in)
{
	IplImage* out = cvCreateImage(cvGetSize(in), IPL_DEPTH_8U, in->nChannels);
	cvSmooth(in, out, CV_GAUSSIAN, 11, 11);
	return out;
}

IplImage* toGrayScale(IplImage* in)
{
	IplImage* out = cvCreateImage(cvGetSize(in), IPL_DEPTH_8U, 1);
	cvCvtColor(in, out, CV_RGB2GRAY);
	cvReleaseImage(&in);
	return out;
}

IplImage* doPyrDown(IplImage* in)
{
	assert(in->width % 2 == 0 && in->height % 2 == 0);
	IplImage* out = cvCreateImage(cvSize(in->width / 2, in->height / 2), in->depth, in->nChannels);
	cvPyrDown(in, out);
	cvReleaseImage(&in);
	return(out);
};

IplImage* doCanny(IplImage* in, double lowThresh, double highThresh, double aperture)
{
	if (in->nChannels != 1) return NULL;
	IplImage* out = cvCreateImage(cvGetSize(in), IPL_DEPTH_8U, 1);
	cvCanny(in, out, lowThresh, highThresh, aperture);
	return(out);
};

void update(void)
{
	cvShowImage("demo", frame);
	out = smooth(frame);
	out = toGrayScale(out);
	out = doCanny(out, low, high, 3);
	cvShowImage("out", out);
	cvReleaseImage(&out);
}

void onTrackbarSlide(int pos)
{
	cvSetCaptureProperty(capture, CV_CAP_PROP_POS_FRAMES, pos);
	frame = cvQueryFrame(capture);
	if (!frame) return;
	update();
}

void onLow(int low)
{
	update();
}

void onHigh(int high)
{
	update();
}

int main(int argc, char** argv) {
	cvNamedWindow("demo", CV_WINDOW_AUTOSIZE);
	cvNamedWindow("out", CV_WINDOW_AUTOSIZE);
	capture = cvCreateFileCapture("drop.avi");
	//capture = cvCreateCameraCapture(0);
	int frames = cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_COUNT);
	if (frames != 0) cvCreateTrackbar("Position", "demo", &position, frames, onTrackbarSlide);
	cvCreateTrackbar("lowThresh", "out", &low, 100, onLow);
	cvCreateTrackbar("highThresh", "out", &high, 300, onHigh);

	frame = cvQueryFrame(capture);
	if (frame) {
		update();
	}

	cvWaitKey(0);
	cvReleaseCapture(&capture);
	cvDestroyWindow("out");
	cvDestroyWindow("demo");
}
