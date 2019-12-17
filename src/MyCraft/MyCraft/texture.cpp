#include "texture.h"
#include "GL/SOIL.h"
#include <iostream>
#include <stdlib.h>


Texture2D::Texture2D()
	: Width(0), Height(0), Internal_Format(GL_RGB), Image_Format(GL_RGB), Wrap_S(GL_REPEAT), Wrap_T(GL_REPEAT), Filter_Min(GL_LINEAR), Filter_Max(GL_LINEAR)
{
	glGenTextures(1, &this->ID);
}

void Texture2D::Generate(GLuint width, GLuint height, unsigned char* data)
{
	/*
		int width, height;
	unsigned char* image = nullptr;
	image = SOIL_load_image(imagePath, &width, &height, 0, SOIL_LOAD_RGB);
	if (image == nullptr)
	{
		std::cout << "image is not existed" << std::endl;
	}
	*/

	glBindTexture(GL_TEXTURE_2D, ID);

	glTexImage2D(GL_TEXTURE_2D, 0, this->Internal_Format, width, height, 0, this->Image_Format, GL_UNSIGNED_BYTE, data);
	
	// glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->Wrap_S);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->Wrap_T);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->Filter_Max);

}

void Texture2D::Bind() const
{
	glBindTexture(GL_TEXTURE_2D, this->ID);
}

Texture2DArray::Texture2DArray()
	: Width(0), Height(0), Internal_Format(GL_RGB), Image_Format(GL_RGB), Wrap_S(GL_REPEAT), Wrap_T(GL_REPEAT), Filter_Min(GL_LINEAR), Filter_Max(GL_LINEAR)
{
	glGenTextures(1, &this->ID);
}
void Texture2DArray::Generate(GLuint width, GLuint height, std::vector<unsigned char*> datas)
{
	glBindTexture(GL_TEXTURE_2D_ARRAY, ID);

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, this->Wrap_S);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, this->Wrap_T);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, this->Filter_Max);

	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, this->Internal_Format, width, height, 
		datas.size(), 0, this->Image_Format, GL_UNSIGNED_BYTE, NULL);

	for (int i = 0; i < datas.size(); i++) {
		glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, width, height, 1, this->Image_Format, GL_UNSIGNED_BYTE, datas[i]);
	}
}

void Texture2DArray::Bind() const
{
	glBindTexture(GL_TEXTURE_2D_ARRAY, this->ID);
}
