#include "screenshot.h"
#include <fileapifromapp.h>




screenshot::screenshot()
{
	winWidth = 1000;
	winHeight = 800;
	arrLen = winWidth * winHeight * 3;
	colorArr = new GLbyte[arrLen];
}

void screenshot::save_img(signed char*& _pt, string& _str)
{
	cv::Mat img;
	vector<cv::Mat> imgPlanes;
	img.create(winHeight, winWidth, CV_8UC3);
	cv::split(img, imgPlanes);

	for (int i = 0; i < winHeight; i++) {
		UCHAR* plane0Ptr = imgPlanes[0].ptr<UCHAR>(i);
		UCHAR* plane1Ptr = imgPlanes[1].ptr<UCHAR>(i);
		UCHAR* plane2Ptr = imgPlanes[2].ptr<UCHAR>(i);
		for (int j = 0; j < winWidth; j++) {
			int k = 3 * (i * winWidth + j);
			plane2Ptr[j] = _pt[k];
			plane1Ptr[j] = _pt[k + 1];
			plane0Ptr[j] = _pt[k + 2];
		}
	}
	cv::merge(imgPlanes, img);
	cv::flip(img, img, 0); // !!!
	cv::imwrite(_str.c_str(), img);
	printf("image saved! \n");
}
