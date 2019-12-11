#pragma once

#include "shader.h"
#include "GL/SOIL.h"

#include <vector>
#include <iostream>

class SkyBox
{
public:
	SkyBox();
	~SkyBox();
	
	void render();
	void init(Shader sky_shader);

private:
	Shader sky_shader;
	GLuint VBO;
	GLuint VAO;
	GLuint cubemapTexture;
	GLuint loadCubemap(std::vector<const char*> faces);
};