#include "cubeRender.h"
#include "resource_manager.h"
#include <iostream>

CubeRender::CubeRender()
{
}

CubeRender::CubeRender(Shader& shader)
{
	this->shader = shader;
	this->initRenderData();
}

void CubeRender::initRenderData()
{
}

GLint CubeRender::createList(Texture2D tex_up, Texture2D tex_side, Texture2D tex_down)
{
	GLint lid = glGenLists(1);

	// std::cout << "generate a lid: " << this->lid << std::endl;
	glNewList(lid, GL_COMPILE);

	glPushMatrix();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// bind the texture

	tex_up.Bind();
	glBegin(GL_QUADS);
	glTexCoord2i(1, 1); glVertex3f(0.5f, 0.5f, 0.5f);
	glTexCoord2i(0, 1); glVertex3f(-0.5f, 0.5f, 0.5f);
	glTexCoord2i(0, 0); glVertex3f(-0.5f, 0.5f, -0.5f);
	glTexCoord2i(1, 0); glVertex3f(0.5f, 0.5f, -0.5);
	glEnd();

	tex_side.Bind();
	glBegin(GL_QUADS);
	glTexCoord2i(1, 1); glVertex3f(0.5f, 0.5f, 0.5f);
	glTexCoord2i(0, 1); glVertex3f(-0.5f, 0.5f, 0.5f);
	glTexCoord2i(0, 0); glVertex3f(-0.5f, -0.5f, 0.5f);
	glTexCoord2i(1, 0); glVertex3f(0.5f, -0.5f, 0.5);
	glEnd();

	tex_side.Bind();
	glBegin(GL_QUADS);
	glTexCoord2i(1, 1); glVertex3f(0.5f, 0.5f, -0.5f);
	glTexCoord2i(0, 1); glVertex3f(-0.5f, 0.5f, -0.5f);
	glTexCoord2i(0, 0); glVertex3f(-0.5f, -0.5f, -0.5f);
	glTexCoord2i(1, 0); glVertex3f(0.5f, -0.5f, -0.5);
	glEnd();

	tex_side.Bind();
	glBegin(GL_QUADS);
	glTexCoord2i(1, 1); glVertex3f(0.5f, 0.5f, 0.5f);
	glTexCoord2i(0, 1); glVertex3f(0.5f, -0.5f, 0.5f);
	glTexCoord2i(0, 0); glVertex3f(0.5f, -0.5f, -0.5f);
	glTexCoord2i(1, 0); glVertex3f(0.5f, 0.5f, -0.5);
	glEnd();

	tex_side.Bind();
	glBegin(GL_QUADS);
	glTexCoord2i(1, 1); glVertex3f(-0.5f, 0.5f, 0.5f);
	glTexCoord2i(0, 1); glVertex3f(-0.5f, -0.5f, 0.5f);
	glTexCoord2i(0, 0); glVertex3f(-0.5f, -0.5f, -0.5f);
	glTexCoord2i(1, 0); glVertex3f(-0.5f, 0.5f, -0.5);
	glEnd();

	tex_down.Bind();
	glBegin(GL_QUADS);
	glTexCoord2i(1, 1); glVertex3f(0.5f, -0.5f, 0.5f);
	glTexCoord2i(0, 1); glVertex3f(-0.5f, -0.5f, 0.5f);
	glTexCoord2i(0, 0); glVertex3f(-0.5f, -0.5f, -0.5f);
	glTexCoord2i(1, 0); glVertex3f(0.5f, -0.5f, -0.5);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();

	glEndList();

	return lid;
}


void CubeRender::drawCube(float x, float y, float z, Texture2D tex_up, Texture2D tex_side, Texture2D tex_down)
{
	std::string id = std::to_string(tex_up.ID) + std::to_string(tex_side.ID) + std::to_string(tex_down.ID);
	GLint lid;

	std::map<std::string, GLint>::iterator it;
	it = this->renderList.find(id);
	if (it != renderList.end())
	{
		lid = it->second;
	}
	else {
		lid = createList(tex_up, tex_side, tex_down);
		renderList.insert(std::pair<std::string, GLint>(id, lid));
	}

	glPushMatrix();
	glTranslatef(x, y, z);

	glCallList(lid);

	glPopMatrix();
}

void CubeRender::drawWireCube(float x, float y, float z)
{
	// glDisable(GL_LIGHTING);
	glPushMatrix();
	glTranslatef(x, y, z);

	this->drawWireCube(1.2f);
	// bind the texture

	glPopMatrix();
	// glEnable(GL_LIGHTING);

}

void CubeRender::drawWireCube(float size)
{
	size = size / 2;
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_QUADS);

	glVertex3f(size, size, size);
	glVertex3f(-size, size, size);
	glVertex3f(-size, -size, size);
	glVertex3f(size, -size, size);

	glVertex3f(size, size, -size);
	glVertex3f(-size, size, -size);
	glVertex3f(-size, -size, -size);
	glVertex3f(size, -size, -size);

	glVertex3f(size, size, size);
	glVertex3f(size, -size, size);
	glVertex3f(size, -size, -size);
	glVertex3f(size, size, -size);

	glVertex3f(-size, size, size);
	glVertex3f(-size, -size, size);
	glVertex3f(-size, -size, -size);
	glVertex3f(-size, size, -size);

	glVertex3f(size, size, size);
	glVertex3f(-size, size, size);
	glVertex3f(-size, size, -size);
	glVertex3f(size, size, -size);

	glVertex3f(size, -size, size);
	glVertex3f(-size, -size, size);
	glVertex3f(-size, -size, -size);
	glVertex3f(size, -size, -size);

	glEnd();

	// 注意这里要还原成 GL_FILL, 保证多边形绘制模式是 GL_FILL
	// 否则天空盒会有问题, 待解决
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}