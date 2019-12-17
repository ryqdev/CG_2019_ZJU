#include "chunk.h"

Chunk::Chunk(int x, int z)
{
	X = x;
	Z = z;
}

Chunk::~Chunk()
{
	for (auto it = blocks.begin(); it != blocks.end(); it++)
		delete it->second;
}

void Chunk::genChunk()
{
	PerlinNoise noise;

	for (int x = 0; x < CHUNK_SIZE; x++)
		for (int z = 0; z < CHUNK_SIZE; z++) {
			// 方块的实际位置
			int xi = X * CHUNK_SIZE + x;
			int zi = Z * CHUNK_SIZE + z;

			// 根据实际位置计算最大高度
			double f = noise.octaveNoise(xi*0.01, zi*0.01, 4, 0.5);
			double g = noise.octaveNoise(-xi*0.01, -zi*0.01, 2, 0.9);
			int mh = g * 32 + 16;
			int h = f * mh;
			int t = 12;
			if (h <= t) {
				h = t;
			}

			for (int y = h-10; y < h-8; y++) {
				int key = x+CHUNK_SIZE*(y+CHUNK_HEIGHT*z);
				Block *block = BlockFactory::getBlock(x+CHUNK_SIZE*this->X, y, z
					+CHUNK_SIZE*this->Z, GRASS);
				blocks.insert(std::make_pair(key, block));
			}
		}

	loaded = true;
}

void Chunk::render(CubeRender *cubeRender)
{
	if (!loaded)
		genChunk();

	for (auto it = blocks.begin(); it != blocks.end(); it++) {
		it->second->Draw(cubeRender);
	}
}

Block *Chunk::getBlock(int x, int y, int z)
{
	std::unordered_map<int, Block*>::iterator it = 
		blocks.find(x + CHUNK_SIZE * (y + CHUNK_HEIGHT * z));
	if (it != blocks.end()) {
		return it->second;
	} else {
		return nullptr;
	}
}

void Chunk::putBlock(int x, int y, int z, BlockType type)
{
	Block* block = BlockFactory::getBlock(x+X*CHUNK_SIZE, y, z+Z*CHUNK_SIZE, type);
	int key = x + CHUNK_SIZE * (y + CHUNK_HEIGHT * z);
	std::unordered_map<int, Block*>::iterator it = blocks.find(key);
	if (it != blocks.end()) {
		it->second = block;
	} else {
		blocks.insert(std::make_pair(key, block));
	}
}

void Chunk::removeBlock(int x, int y, int z)
{
	int key = x + CHUNK_SIZE * (y + CHUNK_HEIGHT * z);
	std::unordered_map<int, Block*>::iterator it = blocks.find(key);
	if (it != blocks.end()) {
		delete it->second;
		blocks.erase(it);
	}
}
