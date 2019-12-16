#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <string>   // for strings
#include <cmath>
#include <GL/glew.h>
#include <GL/SOIL.h>
#include <iostream> 
#include "windows.h"
#include <vector>
#include <cstdio>
#include <ctime>
using namespace std;

static class Screenshot
{
private:
	static void invertY(int w, int h, vector<unsigned char>& buf);

public:
	static void save_img(struct tm* ptminfo);

};