#pragma once

#include "GL/glew.h"
#include "texture.h"
#include "shader.h"
#include "blockType.h"
#include <string>
#include <map>

class CubeRender
{
private:
	Shader lineShader;

	GLuint wireCubeVao, wireCubeVbo;

public:
	CubeRender(Shader& lineShader);

	// 配置 VAO 与 VBO 
	void initRenderData();

	void drawWireCube(glm::mat4 matrix);
};