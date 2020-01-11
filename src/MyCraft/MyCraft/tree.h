#pragma once
#include <vector>
#include"shader.h"
#include <string>
#include <map>
#include "GL/glew.h"
#include "texture.h"
#include "resource_manager.h"

using namespace std;
//#define N_TREE (200)
//#define SIZE (16)
class Tree {
public:
	Tree();
	float position[3];//树木位置
	float leaf[2][2];//两层树叶{r,h}
	float trunk[2];//树干{r,h}
	Tree(float x, float y, float z, float r0 = 0.8, float h0 = 0.8, float r1 = 1, float h1 = 1, float r = 0.2, float h = 1.5);
};


class TreeRender {
private:
	Shader shader;//TODO:使用着色器绘制
	map<std::string, GLint> renderList;//显示列表
	void drawLeaf(int level, Tree& tree, Texture2D& trunk, Texture2D& leaves);//绘制树叶
	void drawTrunk(Tree& tree, Texture2D& trunk, Texture2D& leaves);//绘制树躯干
	// 使用显示列表加快渲染速度
	GLint createList(Tree& tree, Texture2D& trunk, Texture2D& leaves);

public:
	//int treelist[N_TREE][2]; //树的位置 （x,z）
	TreeRender();

	//TODO 用着色器绘制树木
	void DrawTree(Shader& shader, Texture2D& trunk, Texture2D& leaves);
	vector <glm::vec3> points;
	vector <glm::vec2> texs;
	vector <glm::vec3> normals;
	GLuint vaotrunk, vaoleaf;
	GLuint trunkcount, leafcount;
	void  initRenderDataLeaf(Tree& tree);
	void  initRenderDataTrunck(Tree& tree);
	void initRenderData(Tree& tree);//TODO 用着色器的时候，用于配置 VAO 与 VBO 等
	void DrawTree(Tree& tree, Texture2D& trunk, Texture2D& leaves);// 绘制树木

};
