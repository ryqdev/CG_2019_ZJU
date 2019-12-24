#pragma once

#include "chunk.h";
#include "../resource_manager.h"

const int CREATE_RADIUS = 4;
const int DESTROY_RADIUS = 6;

class ChunkManager
{
public:
	ChunkManager() {}
	ChunkManager(Shader& shader);
	~ChunkManager();

	void init(Shader shader);

	int chunked(int x);
	int highest(int x, int z);	// 获取世界坐标(x, z)处最高点的y坐标

	Chunk& getChunk(int X, int Z);	// 获取区块坐标(X, Z)的区块 
	bool chunkExist(int X, int Z);	// 检查区块坐标(X, Z)处是否存在区块

	BlockType getBlock(int x, int y, int z);	// 获取世界坐标(x, y, z)处的方块种类
	void putBlock(int x, int y, int z, BlockType type);	// 把一个种类为type的方块放到世界坐标(x, y, z)处
	void removeBlock(int x, int y, int z);	// 移除世界坐标(x, y, z)处的方块

	void render(glm::vec3 pos, glm::mat4 projViewMatrix);

private:
	Shader shader;

	std::vector<Chunk*> chunks;
};

