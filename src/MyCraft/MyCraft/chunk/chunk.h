#pragma once

#include <vector>

#include <GL/glew.h>
#include <unordered_map>

#include "../blockType.h"
#include "perlinNoise.h"
#include "../blockFactory.h"

const int CHUNK_SIZE = 16;
const int CHUNK_HEIGHT = 128;

class Chunk
{
public:
	int X, Z;	// 区块位置

	Chunk(int x, int z) ;
	~Chunk();

	void genChunk();	// 根据X，Z生成区块或从文件载入
	void render(CubeRender *cubeRender);	

	int highest(int x, int z);

	Block *getBlock(int x, int y, int z);
	void putBlock(int x, int y, int z, BlockType type);
	void removeBlock(int x, int y, int z);
private:
	bool loaded = false;
	std::unordered_map<int, Block*> blocks;
};

