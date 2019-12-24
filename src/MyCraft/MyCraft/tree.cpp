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

//纹理方式绘制树木
TreeRender::TreeRender(){
	//树木位置
	for (int i = 0; i < N_TREE; i++) {
		treelist[i][0] = rand() % 16;
		treelist[i][1] = rand() % 16;
	}
}

//着色器方式绘制树木
TreeRender::TreeRender(Shader& shader) {
	//树木位置
	for (int i = 0; i < N_TREE; i++) {
		treelist[i][0] = rand() % 16;
		treelist[i][1] = rand() % 16;
	}
	this->shader = shader;
}

//TODO 用着色器的时候，用于配置 VAO 与 VBO 等
void TreeRender::initRenderData() {
	//TODO 
	//用着色器的时候，用于配置 VAO 与 VBO 等
}

GLint TreeRender::createList(Tree& tree,Texture2D& trunk,Texture2D& leaves)
{
	GLint lid = glGenLists(1);//创建一个显示列表

	// std::cout << "generate a lid: " << this->lid << std::endl;
	glNewList(lid, GL_COMPILE);

	//画第一层树叶
	glPushMatrix();
	glTranslatef(0, tree.trunk[1] + tree.leaf[1][1], 0);
	drawLeaf(0,tree,trunk,leaves);
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
	
	for (float angle =0; angle <= 2 * PI; angle +=1) {
		next = angle + 1;
		if (next > 2 * PI) next = 0;

		//画圆锥侧面
		glTexCoord2i(0.5, 0);
		glVertex3f(0, tree.leaf[level][1], 0);//0

		glTexCoord2i(0, 1);
		glVertex3f(r*sin(angle), 0, r*cos(angle));//1

		glTexCoord2i(1, 1);
		glVertex3f(r*sin(next), 0, r*cos(next));//2

		//画圆锥底面
		glTexCoord2i(0.5, 0);
		glVertex3f(0, 0, 0);//0

		glTexCoord2i(0, 1);
		glVertex3f(r*sin(angle), 0, r*cos(angle));//1

		glTexCoord2i(1, 1);
		glVertex3f(r*sin(next), 0, r*cos(next));//2

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

	glBegin(GL_QUADS);
	//画圆柱侧面
	for (float angle = 0; angle <= 2 * PI; angle += 2) {
		next = angle +2;
		if (next > 2 * PI) next = 0;

		glTexCoord2i(0, 0);
		glVertex3f(r*cos(angle), tree.trunk[1], r*sin(angle));

		glTexCoord2i(0, 1);
		glVertex3f(r*cos(angle), 0, r*sin(angle));

		glTexCoord2i(1, 1);
		glVertex3f(r*cos(next), 0, r*sin(next));

		glTexCoord2i(1, 0);
		glVertex3f(r*cos(next), tree.trunk[1], r*sin(next));
	}
	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
	glPopMatrix();
}

void TreeRender::DrawTree(Tree& tree, Texture2D& trunk, Texture2D& leaves) {
	//生成树的独特ID
	std::string id = to_string(trunk.ID) + to_string(leaves.ID) + to_string(tree.leaf[0][0])+ to_string(tree.leaf[0][1])
		+ to_string(tree.leaf[1][0])+ to_string(tree.leaf[1][1])+ to_string(tree.trunk[0])+ to_string(tree.trunk[1]);
	
	GLint lid;
	std::map<std::string, GLint>::iterator it;
	it = this->renderList.find(id);
	if (it != renderList.end())
	{
		lid = it->second;
	}
	else {	
		lid = createList(tree,trunk,leaves);
		//cout << lid << endl;
		renderList.insert(std::pair<std::string, GLint>(id, lid));
	}

	glPushMatrix();
	glTranslatef(tree.position[0], tree.position[1], tree.position[2]);

	glCallList(lid);

	glPopMatrix();
}
