#pragma once

#include "GL/glew.h"
#include "texture.h"
#include "shader.h"
#include "blockType.h"
#include <string>
#include <map>

// 一个渲染器, 用来渲染一个有纹理的 1x1x1 的立方体
// 方块对象通过调用该渲染器来渲染


// TODO: 修改为着色器方式渲染
// https://learnopengl-cn.github.io/06%20In%20Practice/2D-Game/03%20Rendering%20Sprites/

class CubeRender
{
private:
	Shader shader;

	// 使用显示列表加快渲染速度
	std::map<std::string, GLint> renderList;

	GLint createList(Texture2D tex_up, Texture2D tex_side, Texture2D tex_down);

	void drawWireCube(float x);

public:

	CubeRender();
	CubeRender(Shader& shader);

	// 用着色器的时候，用于配置 VAO 与 VBO 等
	void initRenderData();

	// render to the screen
	void drawCube(float x, float y, float z, Texture2D tex_up, Texture2D tex_side, Texture2D tex_down);

	void drawWireCube(float x, float y, float z);
};