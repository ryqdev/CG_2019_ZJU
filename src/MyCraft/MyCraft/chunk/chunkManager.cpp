#include "chunkManager.h"

ChunkManager::ChunkManager(Shader& shader)
{
	this->shader = shader;
}

ChunkManager::~ChunkManager()
{
}

void ChunkManager::init(Shader shader)
{
	this->shader = shader;
}

int ChunkManager::chunked(int x)
{
	return floor(float(x) / CHUNK_SIZE);
}

// 获取世界坐标(x, z)处最高点的y坐标
int ChunkManager::highest(int x, int z)
{
	int X = chunked(x), Z = chunked(z);

	return getChunk(X, Z).highest(x-X*CHUNK_SIZE, z-Z*CHUNK_SIZE);
}

// 获取区块坐标(X, Z)的区块
Chunk& ChunkManager::getChunk(int X, int Z)
{
	auto it = chunks.begin();
	for (; it != chunks.end(); it++)
		if ((*it)->X == X && (*it)->Z == Z)
			return **it;
	
	// 若区块不存在，则初始化一个区块并返回
	Chunk* chunk = new Chunk(X, Z);
	chunk->genChunk();
	chunks.push_back(chunk);
	return *chunk;
}

// 检查区块坐标(X, Z)处是否存在区块
bool ChunkManager::chunkExist(int X, int Z)
{
	auto it = chunks.begin();
	for (; it != chunks.end(); it++)
		if ((*it)->X == X && (*it)->Z == Z)
			return true;
	return false;
}

// 获取世界坐标(x, y, z)处的方块种类
BlockType ChunkManager::getBlock(int x, int y, int z)
{
	int X = chunked(x), Z = chunked(z);

	return getChunk(X, Z).getBlock(x-X*CHUNK_SIZE, y, z-Z*CHUNK_SIZE);
}

// 把一个种类为type的方块放到世界坐标(x, y, z)处
void ChunkManager::putBlock(int x, int y, int z, BlockType type)
{
	int X = chunked(x), Z = chunked(z);

	getChunk(X, Z).putBlock(x-X*CHUNK_SIZE, y, z-Z*CHUNK_SIZE, type);
}

// 移除世界坐标(x, y, z)处的方块
void ChunkManager::removeBlock(int x, int y, int z)
{
	int X = chunked(x), Z = chunked(z);

	getChunk(X, Z).removeBlock(x-X*CHUNK_SIZE, y, z-Z*CHUNK_SIZE);
}

// 使用render绘制camera周围的区块
void ChunkManager::render(glm::vec3 pos, glm::mat4 projViewMatrix)
{
	int cX = chunked(pos.x);
	int cZ = chunked(pos.z);

	// 删掉位于摄像机的摧毁半径外的区块
	for (auto it = chunks.begin(); it != chunks.end();) {
		if (std::max(abs((*it)->X-cX), abs((*it)->Z-cZ)) > DESTROY_RADIUS) {
			delete *it;
			it = chunks.erase(it);
		} else {
			it++;
		}
	}

	glEnable(GL_CULL_FACE);

	shader.Use();
	shader.SetInteger("tex", 0);
	shader.SetMatrix4("matrix", projViewMatrix);

	Texture2DArray textureArray = ResourceManager::GetTextureArray("blocks");
	textureArray.Bind();

	for (int i = cX-CREATE_RADIUS; i <= cX+CREATE_RADIUS; i++) {
		for (int j = cZ-CREATE_RADIUS; j <= cZ+CREATE_RADIUS; j++) {
			Chunk *c = &getChunk(i, j);

			if (!c->isLoaded()) 
				c->genChunk();

			if (c->isDirty())
				c->genBuffer();

			glBindVertexArray(c->vao);
			glDrawArrays(GL_TRIANGLES, 0, c->vertsNum);
		}
	}
}