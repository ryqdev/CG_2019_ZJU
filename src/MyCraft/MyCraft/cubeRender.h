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

	CubeRender();
	CubeRender(Shader& lineShader);

	// ≈‰÷√ VAO ”Î VBO 
	void initRenderData();

	void drawWireCube(int x, int y, int z, glm::mat4 matrix);
};