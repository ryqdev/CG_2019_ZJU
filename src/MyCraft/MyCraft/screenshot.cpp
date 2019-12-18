#include "screenshot.h"


void Screenshot::invertY(int w, int h, vector<unsigned char>& buf)
{
	std::vector<unsigned char> tmp_rgbdata(buf); // Create a copy of the data

	for (int i = h - 1; i >= 0; i--) // Count backwards in order to flip
	{
		memcpy(&buf[i * w * 3],                    // address of destination
			&tmp_rgbdata[(h - i - 1) * w * 3], // address of source
			w * 3 * sizeof(unsigned char));        // number of bytes to copy
	}
	tmp_rgbdata.clear(); // Clear the temporary array
}

void Screenshot::save_img(struct tm* ptminfo)
{


	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	GLint viewPort[4] = { 0 };
	glGetIntegerv(GL_VIEWPORT, viewPort);

	vector<unsigned char> buf(viewPort[2] * viewPort[3] * 3);

	glReadPixels(viewPort[0], viewPort[1], viewPort[2], viewPort[3], GL_RGB, GL_UNSIGNED_BYTE, &buf[0]);

	invertY(viewPort[2], viewPort[3], buf);

	string year = to_string(ptminfo->tm_year + 1900);
	string month = to_string(ptminfo->tm_mon + 1);
	string day = to_string(ptminfo->tm_mday);
	string hour = to_string(ptminfo->tm_hour);
	string minute = to_string(ptminfo->tm_min);
	string second = to_string(ptminfo->tm_sec);
	string d = ".\\screenshot\\img" + year + "_" + month + "_" + day + "_" + hour + "_" + minute + "_" +  second + ".jpg";
	int err = SOIL_save_image
	(
		d.c_str(),
		SOIL_SAVE_TYPE_BMP,
		viewPort[2], viewPort[3], 3,
		&buf[0]
	);
	if (err == 0) {
		cout << "can't save the image" << endl;
	}
	else {
		cout << "image is save into " << d << endl;
	}
}