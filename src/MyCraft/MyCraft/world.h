#pragma once

#include "GL/glew.h"
#include "block.h"
#include "skyBox.h"
#include "cubeRender.h"
#include "blockFactory.h"
#include "chunk/chunk.h"

#include <vector>
#include<unordered_map>

const int WIDTH = 100;

class World
{
private:
	CubeRender* cubeRender;

	// 天空盒对象
	SkyBox skyBox;
	// 要渲染的区块
	std::vector<Chunk*> chunks;

	int chunked(int x);

	//世界物体材质
	float mAmbientMaterial[4], mDiffuseMaterial[4], mSpecularMaterial[4];

public:

	// 放一个 Block 在指定坐标中
	void put_block(int x, int y, int z, BlockType type);

	void clear_block(int x, int y, int z);

	// 拾取该block， 会将 BLock 的isSelect值设为true
	void pick_block(int x, int y, int z);
	// 这是取消选中
	void unpick_block(int x, int y, int z);
	
	char get_map(int x, int y, int z);

	int highest(int x, int z);

	// 用chunk坐标寻找chunk
	Chunk *findChunk(int x, int z);

	World();
	~World();

	// 从文件中加载世界数据, 创建相应的方块对象
	void Load();

	// 初始化世界生成，准备着色器，纹理等
	void init();

	// 渲染整个世界
	void render();

	//设置世界物体反射材质
	void SetAmbientMaterial(float r, float g, float b, float a);
	void SetDiffuseMaterial(float r, float g, float b, float a);
	void SetSpecularMaterial(float r, float g, float b, float a);
};