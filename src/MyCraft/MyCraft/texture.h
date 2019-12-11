#pragma once
#include "GL/glut.h"
#define GL_CLAMP_TO_EDGE    0x812F

class Texture2D
{
private:

	// Texture image dimensions
	GLuint Width, Height; // Width and height of loaded image in pixels

	// Texture configuration
	GLuint Wrap_S; // Wrapping mode on S axis
	GLuint Wrap_T; // Wrapping mode on T axis
	GLuint Filter_Min; // Filtering mode if texture pixels < screen pixels
	GLuint Filter_Max; // Filtering mode if texture pixels > screen pixels
public:
	// Holds the ID of the texture object, used for all texture operations to reference to this particlar texture
	GLuint ID;
	// Texture Format
	GLuint Internal_Format; // Format of texture object
	GLuint Image_Format; // Format of loaded image

	Texture2D();
	// Generates texture from image data
	void Generate(GLuint width, GLuint height, unsigned char* data);

	// Binds the texture as the current active GL_TEXTURE_2D texture object
	void Bind() const;
};