// pointProcess.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <vector>

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
	Mat src, dst;
	src = imread(argv[1], CV_LOAD_IMAGE_COLOR);		// Load original image
	if (src.data == NULL)
	{
		cout << "Can not find image!" << endl;
		return -1;
	}
	vector<Mat> rgb_planes;
	split(src, rgb_planes);
	int histSize = 256;  // from 0 to 255
	// Set the ranges(for R, G, B)
	float range[] = { 0, 256 };
	const float *histRange = range;
	bool uniform = true;
	bool accumulate = false;
	Mat hists[3];

	for (int i = 0; i < 3; ++i)
	{
		calcHist(&rgb_planes[i], 1, 0, Mat(), hists[i], 1, &histSize, &histRange, uniform, accumulate);
	}

	// 创建直方图画布
	int hist_w = 400; int hist_h = 400;
	int bin_w = cvRound((double)hist_w / histSize);

	Mat histImage(hist_w, hist_h, CV_8UC3, Scalar(0, 0, 0));
	for (int i = 0; i < 256; ++i)
	{
		cout << (int)hists[0].data[i] << " ";
	}
	cout << endl;
	// 将直方图归一化到范围 [ 0, histImage.rows ]
	for (int i = 0; i < 3; ++i)
	{
		normalize(hists[i], hists[i], 0, histImage.rows, NORM_MINMAX, -1, Mat());
	}
	
	// 在直方图画布上画出直方图
	for (int i = 1; i < histSize; i++)
	{
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(hists[0].at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(hists[0].at<float>(i))),
			Scalar(0, 0, 255), 2, 8, 0);
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(hists[1].at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(hists[1].at<float>(i))),
			Scalar(0, 255, 0), 2, 8, 0);
		line(histImage, Point(bin_w*(i - 1), hist_h - cvRound(hists[2].at<float>(i - 1))),
			Point(bin_w*(i), hist_h - cvRound(hists[2].at<float>(i))),
			Scalar(255, 0, 0), 2, 8, 0);
		//rectangle(histImage, Rect(Point(bin_w * (i - 1), hist_h - cvRound(hists[0].at<float>(i - 1))), Point(bin_w * i, hist_h)), Scalar(0, 0, 255));
	}
	
	// 显示直方图
	namedWindow("calcHist Demo", CV_WINDOW_AUTOSIZE);
	imshow("calcHist Demo", histImage);

	waitKey(0);
	return 0;
}