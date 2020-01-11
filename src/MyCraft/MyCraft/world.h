#pragma once

#include "GL/glew.h"
#include "cubeRender.h"
#include "robot.h"
#include "skyBox.h"
#include "tree.h"
#include "chunk/chunk.h"

#include <vector>
#include<unordered_map>

class Camera;

class World
{
private:
	CubeRender* cubeRender;
	TreeRender* treeRender;
	RobotRender* robotRender;
	bool picked = false;
	glm::vec3 pickedBlock;

	SkyBox skyBox;	// 天空盒对象
	unordered_map<int, Chunk*> chunks;
	Shader sence, chunkshader;

	Texture2D trunk;
	Texture2D leaves;

	Texture2D earth, sun;
	Texture2D moon;

	int chunked(int x);
	Chunk* findChunk(int x, int z);

	//世界物体材质
	float mAmbientMaterial[4], mDiffuseMaterial[4], mSpecularMaterial[4];

public:
	World();
	~World();

	// 修改需要绘制的线框位置
	void pick_block(int x, int y, int z);
	void unpick_block();

	BlockType get_block(int x, int y, int z);

	// 放一个 Block 在指定坐标中
	void put_block(int x, int y, int z, BlockType type);

	void remove_block(int x, int y, int z);

	int highest(int x, int z);

	// 初始化世界生成，准备着色器，纹理等
	void init();

	// 渲染整个世界
	void render(glm::mat4 projection, glm::mat4 view, glm::vec3 cameraPos);
};
