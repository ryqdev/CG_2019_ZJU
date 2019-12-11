#pragma once

#include "GL/glew.h"
#include "block.h"
#include "skyBox.h"
#include "cubeRender.h"

#include <vector>
#include<unordered_map>

const int WIDTH = 100;

class World
{
private:
	CubeRender* cubeRender;

	// 天空盒对象
	SkyBox skyBox;

	void set_map(int x, int y, int z);

	char map[WIDTH][WIDTH][WIDTH];

	// 要渲染的方块
	// std::vector<Block> blocks;
	// 尝试用 map 存储
	std::unordered_map<int, Block> blockMap;

	// 根据世界坐标，计算对应的 map 中的键值
	int get_block_id(int x, int y, int z);

public:

	// 放一个 Block 在指定坐标中
	void put_block(int x, int y, int z);

	// 拾取该block， 会将 BLock 的isSelect值设为true
	void pick_block(int x, int y, int z);
	// 这是取消选中
	void unpick_block(int x, int y, int z);
	
	char get_map(int x, int y, int z);

	World();
	~World();

	// 从文件中加载世界数据, 创建相应的方块对象
	void Load();

	// 初始化世界生成，准备着色器，纹理等
	void init();

	// 渲染整个世界
	void render();
};