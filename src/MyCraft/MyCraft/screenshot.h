#pragma once
#include <string>   // for strings
#include <cmath>
#include <opencv2/video.hpp> 
#include <opencv2/highgui.hpp> 
#include <iostream> 
#include "windows.h"
#include "glut.h"
#include <cstdio>
#include <ctime>
using namespace std;



class screenshot
{
public:
	int winWidth;
	int winHeight;
	int arrLen;
	signed char* colorArr;
	screenshot();
	void save_img(signed char*& _pt, string& _str);
};

