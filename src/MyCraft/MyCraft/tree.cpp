#include "resource_manager.h"
#include <iostream>
#include "tree.h"
#include "texture.h"
#include <math.h>

#define PI 3.1415926536
Tree::Tree(float x, float y, float z, float r0, float h0, float r1, float h1, float r, float h) {
	position[0] = x; position[1] = y; position[2] = z;
	leaf[0][0] = r0; leaf[0][1] = h0;
	leaf[1][0] = r1; leaf[1][1] = h1;
	trunk[0] = r; trunk[1] = h;
}
Tree::Tree()
{
	position[0] = 0; position[1] = 0; position[2] = 0;
	leaf[0][0] = 0.8; leaf[0][1] = 0.8;
	leaf[1][0] = 1; leaf[1][1] = 1;
	trunk[0] = 0.2; trunk[1] = 1.5;

}
//纹理方式绘制树木
TreeRender::TreeRender() {


}

//着色器方式绘制树木
void TreeRender::DrawTree(Shader& shader, Texture2D& trunk, Texture2D& leaves) {
	//树木位置
	//树木位置

//	std::cout << "vaotrunk" << vaotrunk << std::endl;
//	std::cout << "vaoleaf" << vaoleaf << std::endl;

//	this->shader = shader;
	shader.Use();
	trunk.Bind();
	glBindVertexArray(vaotrunk);
	glDrawArrays(GL_TRIANGLES, 0, trunkcount);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	leaves.Bind();
	glBindVertexArray(vaoleaf);

	glDrawArrays(GL_TRIANGLES, 0, leafcount);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	//glUseProgram(0);

}
glm::vec3 calNormals(glm::vec3 p0, glm::vec3 p1, glm::vec3 p2)
{
	glm::vec3 p01 = p0 - p1;
	glm::vec3 p02 = p0 - p2;
	glm::vec3 p = glm::cross(p01, p02);
	return p;
}

//TODO 用着色器的时候，用于配置 VAO 与 VBO 等
void TreeRender::initRenderData(Tree& tree) {
	//TODO 
	//用着色器的时候，用于配置 VAO 与 VBO 等

	initRenderDataTrunck(tree);
	initRenderDataLeaf(tree);

}
void TreeRender::initRenderDataTrunck(Tree& tree) {
	//TODO 
	//用着色器的时候，用于配置 VAO 与 VBO 等




	float r = tree.trunk[0];
	float next;
	//树干
	for (float angle = 0; angle <= 2 * PI; angle += 1.0 / 180) {
		next = angle + 1.0 / 180;
		if (next > 2 * PI) next = 0;

		texs.push_back(glm::vec2(0, 0));
		points.push_back(glm::vec3(r * cos(angle), tree.trunk[1], r * sin(angle)));
		//	glTexCoord2i(0, 0);
		//glVertex3f(r * cos(angle), tree.trunk[1], r * sin(angle));
		texs.push_back(glm::vec2(0, 1));
		points.push_back(glm::vec3(r * cos(angle), 0, r * sin(angle)));
		//glTexCoord2i(0, 1);
		//glVertex3f(r * cos(angle), 0, r * sin(angle));
		texs.push_back(glm::vec2(1, 1));
		points.push_back(glm::vec3(r * cos(next), 0, r * sin(next)));

		texs.push_back(glm::vec2(0, 0));
		points.push_back(glm::vec3(r * cos(angle), tree.trunk[1], r * sin(angle)));
		texs.push_back(glm::vec2(1, 1));
		points.push_back(glm::vec3(r * cos(next), 0, r * sin(next)));
		//glTexCoord2i(1, 1);
		//glVertex3f(r * cos(next), 0, r * sin(next));
		texs.push_back(glm::vec2(1, 0));
		points.push_back(glm::vec3(r * cos(next), tree.trunk[1], r * sin(next)));
		//glTexCoord2i(1, 0);
		//glVertex3f(r * cos(next), tree.trunk[1], r * sin(next));
	}
	//树叶


	//计算法线
	for (int i = 0; i < points.size(); i++)
	{
		normals.push_back(glm::vec3(0, 0, 0));
	}

	for (int i = 0; i < points.size(); i = i + 3)
	{
		normals[i] += calNormals(points[i], points[i + 1], points[i + 2]);
		normals[i + 1] += calNormals(points[i], points[i + 1], points[i + 2]);
		normals[i + 2] += calNormals(points[i], points[i + 1], points[i + 2]);
	}

	for (int i = 0; i < points.size(); i++)
	{
		normals[i] = glm::normalize(normals[i]);
	}


	std::vector<float>  data;
	for (int i = 0; i < points.size(); i++)
	{
		data.push_back(points[i].x);
		data.push_back(points[i].y);
		data.push_back(points[i].z);

		data.push_back(normals[i].x);
		data.push_back(normals[i].y);
		data.push_back(normals[i].z);

		data.push_back(texs[i].x);
		data.push_back(texs[i].y);

	}
	//绑定vao 初始化

	// vao与vbo
	glGenVertexArrays(1, &vaotrunk);
	glBindVertexArray(vaotrunk);
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(6 * sizeof(float)));
	glBindVertexArray(0);
	trunkcount = points.size();
	points.clear();
	texs.clear();
	normals.clear();
	data.clear();

}
//出示画叶子
void TreeRender::initRenderDataLeaf(Tree& tree) {
	//TODO 
	//用着色器的时候，用于配置 VAO 与 VBO 等




	float next;
	//树叶
	float r = tree.leaf[0][0];
	//glTranslatef(0, tree.trunk[1] + tree.leaf[1][1], 0);

	for (float angle = 0; angle <= 2 * PI; angle += 1 / 180.0) {
		next = angle + 1.0 / 180;
		if (next > 2 * PI) next = 0;

		////画圆锥侧面
		//glTexCoord2i(0.5, 0);
		//glVertex3f(0, tree.leaf[0][1] + tree.trunk[1] + tree.leaf[1][1], 0);//0
		texs.push_back(glm::vec2(0.5, 0));
		points.push_back(glm::vec3(0, tree.leaf[0][1] + tree.trunk[1] + tree.leaf[1][1], 0));
		//glTexCoord2i(0, 1);
		//glVertex3f(r * sin(angle), tree.trunk[1] + tree.leaf[1][1], r * cos(angle));//1
		texs.push_back(glm::vec2(0, 1));
		points.push_back(glm::vec3(r * sin(angle), tree.trunk[1] + tree.leaf[1][1], r * cos(angle)));
		//glTexCoord2i(1, 1);
		//glVertex3f(r * sin(next), tree.trunk[1] + tree.leaf[1][1], r * cos(next));//2
		texs.push_back(glm::vec2(1, 1));
		points.push_back(glm::vec3(r * sin(next), tree.trunk[1] + tree.leaf[1][1], r * cos(next)));
		////画圆锥底面
		//glTexCoord2i(0.5, 0);
		//glVertex3f(0, tree.trunk[1] + tree.leaf[1][1], 0);//0
		texs.push_back(glm::vec2(0.5, 0));
		points.push_back(glm::vec3(0, tree.trunk[1] + tree.leaf[1][1], 0));
		//glTexCoord2i(0, 1);
		//glVertex3f(r * sin(angle), tree.trunk[1] + tree.leaf[1][1], r * cos(angle));//1
		texs.push_back(glm::vec2(0, 1));
		points.push_back(glm::vec3(r * sin(angle), tree.trunk[1] + tree.leaf[1][1], r * cos(angle)));
		//glTexCoord2i(1, 1);
		//glVertex3f(r * sin(next), tree.trunk[1] + tree.leaf[1][1], r * cos(next));//2
		texs.push_back(glm::vec2(1, 1));
		points.push_back(glm::vec3(r * sin(next), tree.trunk[1] + tree.leaf[1][1], r * cos(next)));
	}

	r = tree.leaf[1][0];

	for (float angle = 0; angle <= 2 * PI; angle += 1.0 / 180) {
		next = angle + 1.0 / 180;
		if (next > 2 * PI) next = 0;

		////画圆锥侧面
		//glTexCoord2i(0.5, 0);
		//glVertex3f(0, tree.leaf[1][1] + tree.trunk[1], 0);//0
		texs.push_back(glm::vec2(0.5, 0));
		points.push_back(glm::vec3(0, tree.leaf[1][1] + tree.trunk[1], 0));
		//glTexCoord2i(0, 1);
		//glVertex3f(r * sin(angle), tree.trunk[1], r * cos(angle));//1
		texs.push_back(glm::vec2(0, 1));
		points.push_back(glm::vec3(r * sin(angle), tree.trunk[1], r * cos(angle)));
		//glTexCoord2i(1, 1);
		//glVertex3f(r * sin(next), tree.trunk[1], r * cos(next));//2
		texs.push_back(glm::vec2(1, 1));
		points.push_back(glm::vec3(r * sin(next), tree.trunk[1], r * cos(next)));
		////画圆锥底面
		//glTexCoord2i(0.5, 0);
		//glVertex3f(0, tree.trunk[1], 0);//0
		texs.push_back(glm::vec2(0.5, 0));
		points.push_back(glm::vec3(0, tree.trunk[1], 0));
		//glTexCoord2i(0, 1);
		//glVertex3f(r * sin(angle), tree.trunk[1], r * cos(angle));//1
		texs.push_back(glm::vec2(0, 1));
		points.push_back(glm::vec3(r * sin(angle), tree.trunk[1], r * cos(angle)));
		//glTexCoord2i(1, 1);
		//glVertex3f(r * sin(next), tree.trunk[1], r * cos(next));//2
		texs.push_back(glm::vec2(1, 1));
		points.push_back(glm::vec3(r * sin(next), tree.trunk[1], r * cos(next)));
	}

	//计算法线
	for (int i = 0; i < points.size(); i++)
	{
		normals.push_back(glm::vec3(0, 0, 0));
	}

	for (int i = 0; i < points.size(); i = i + 3)
	{
		normals[i] += calNormals(points[i], points[i + 1], points[i + 2]);
		normals[i + 1] += calNormals(points[i], points[i + 1], points[i + 2]);
		normals[i + 2] += calNormals(points[i], points[i + 1], points[i + 2]);
	}

	for (int i = 0; i < points.size(); i++)
	{
		normals[i] = glm::normalize(normals[i]);
	}


	std::vector<float>  data;
	for (int i = 0; i < points.size(); i++)
	{
		data.push_back(points[i].x);
		data.push_back(points[i].y);
		data.push_back(points[i].z);

		data.push_back(normals[i].x);
		data.push_back(normals[i].y);
		data.push_back(normals[i].z);

		data.push_back(texs[i].x);
		data.push_back(texs[i].y);

	}
	//绑定vao 初始化

	// vao与vbo
	glGenVertexArrays(1, &vaoleaf);
	glBindVertexArray(vaoleaf);
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(6 * sizeof(float)));
	glBindVertexArray(0);
	//删掉数据
	leafcount = points.size();
	points.clear();
	texs.clear();
	normals.clear();
	data.clear();

}
GLint TreeRender::createList(Tree& tree, Texture2D& trunk, Texture2D& leaves)
{
	GLint lid = glGenLists(1);//创建一个显示列表

	// std::cout << "generate a lid: " << this->lid << std::endl;
	glNewList(lid, GL_COMPILE);

	//画第一层树叶
	glPushMatrix();
	glTranslatef(0, tree.trunk[1] + tree.leaf[1][1], 0);
	drawLeaf(0, tree, trunk, leaves);
	glPopMatrix();

	//画第二层树叶
	glPushMatrix();
	glTranslatef(0, tree.trunk[1], 0);
	drawLeaf(1, tree, trunk, leaves);
	glPopMatrix();

	//画树干
	glPushMatrix();
	drawTrunk(tree, trunk, leaves);
	glPopMatrix();

	glEndList();

	return lid;
}

void TreeRender::drawLeaf(int level, Tree& tree, Texture2D& trunk, Texture2D& leaves) {
	GLfloat angle;
	float r = tree.leaf[level][0];
	float next;

	glPushMatrix();
	//绑定纹理
	leaves.Bind();


	glBegin(GL_TRIANGLES);

	for (float angle = 0; angle <= 2 * PI; angle += 1) {
		next = angle + 1;
		if (next > 2 * PI) next = 0;

		//画圆锥侧面
		glTexCoord2i(0.5, 0);
		glVertex3f(0, tree.leaf[level][1], 0);//0

		glTexCoord2i(0, 1);
		glVertex3f(r * sin(angle), 0, r * cos(angle));//1

		glTexCoord2i(1, 1);
		glVertex3f(r * sin(next), 0, r * cos(next));//2

		//画圆锥底面
		glTexCoord2i(0.5, 0);
		glVertex3f(0, 0, 0);//0

		glTexCoord2i(0, 1);
		glVertex3f(r * sin(angle), 0, r * cos(angle));//1

		glTexCoord2i(1, 1);
		glVertex3f(r * sin(next), 0, r * cos(next));//2

	}

	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();

}

void TreeRender::drawTrunk(Tree& tree, Texture2D& trunk, Texture2D& leaves) {
	float next;
	glPushMatrix();
	trunk.Bind();
	float r = tree.trunk[0];

	//	glBegin(GL_QUADS);
		//画圆柱侧面
	for (float angle = 0; angle <= 2 * PI; angle += 2) {
		next = angle + 2;
		if (next > 2 * PI) next = 0;

		texs.push_back(glm::vec2(0, 0));
		points.push_back(glm::vec3(r * cos(angle), tree.trunk[1], r * sin(angle)));
		//	glTexCoord2i(0, 0);
		//glVertex3f(r * cos(angle), tree.trunk[1], r * sin(angle));
		texs.push_back(glm::vec2(0, 1));
		points.push_back(glm::vec3(r * cos(angle), 0, r * sin(angle)));
		//glTexCoord2i(0, 1);
		//glVertex3f(r * cos(angle), 0, r * sin(angle));
		texs.push_back(glm::vec2(1, 1));
		points.push_back(glm::vec3(r * cos(next), 0, r * sin(next)));

		texs.push_back(glm::vec2(0, 0));
		points.push_back(glm::vec3(r * cos(angle), tree.trunk[1], r * sin(angle)));
		texs.push_back(glm::vec2(1, 1));
		points.push_back(glm::vec3(r * cos(next), 0, r * sin(next)));
		//glTexCoord2i(1, 1);
		//glVertex3f(r * cos(next), 0, r * sin(next));
		texs.push_back(glm::vec2(1, 0));
		points.push_back(glm::vec3(r * cos(next), tree.trunk[1], r * sin(next)));
		//glTexCoord2i(1, 0);
		//glVertex3f(r * cos(next), tree.trunk[1], r * sin(next));
	}
	//	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
}

void TreeRender::DrawTree(Tree& tree, Texture2D& trunk, Texture2D& leaves) {
	//生成树的独特ID
	std::string id = to_string(trunk.ID) + to_string(leaves.ID) + to_string(tree.leaf[0][0]) + to_string(tree.leaf[0][1])
		+ to_string(tree.leaf[1][0]) + to_string(tree.leaf[1][1]) + to_string(tree.trunk[0]) + to_string(tree.trunk[1]);

	GLint lid;
	std::map<std::string, GLint>::iterator it;
	it = this->renderList.find(id);
	if (it != renderList.end())
	{
		lid = it->second;
	}
	else {
		lid = createList(tree, trunk, leaves);
		//cout << lid << endl;
		renderList.insert(std::pair<std::string, GLint>(id, lid));
	}

	glPushMatrix();
	glTranslatef(tree.position[0], tree.position[1], tree.position[2]);

	glCallList(lid);

	glPopMatrix();
}
