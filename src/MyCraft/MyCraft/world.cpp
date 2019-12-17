#include "world.h"
#include "resource_manager.h"


World::World()
{
}

Chunk* World::findChunk(int x, int z)
{
	for (auto it = chunks.begin(); it != chunks.end(); it++) {
		if ((*it)->X == x && (*it)->Z == z)
			return (*it);
	}
	return nullptr;
}

int World::chunked(int x) {
	return floor(float(x) / CHUNK_SIZE);
}

char World::get_map(int x, int y, int z)
{
	int X = chunked(x), Z = chunked(z);
	Chunk *c = findChunk(X, Z);
	if (!c) return 0;

	return c->getBlock(x-X*CHUNK_SIZE, y, z-Z*CHUNK_SIZE) != nullptr;
}

World::~World()
{
	// 释放渲染器
	if (cubeRender != nullptr)
		delete cubeRender;
}

void World::Load()
{
	for (int i = -1; i < 1; i++) {
		for (int j = -1; j < 1; j++) {
			Chunk *chunk = new Chunk(i, j);
			chunk->genChunk();
			chunks.push_back(chunk);
		}
	}

	this->skyBox = SkyBox();
}

void World::pick_block(int x, int y, int z)
{
	int X = chunked(x), Z = chunked(z);
	Chunk *c = findChunk(X, Z);
	if (!c) return;

	Block *b = c->getBlock(x-X*CHUNK_SIZE, y, z-Z*CHUNK_SIZE);
	if (b)
		b->select_block();
}

void World::unpick_block(int x, int y, int z)
{
	int X = chunked(x), Z = chunked(z);
	Chunk *c = findChunk(X, Z);
	if (!c) return;

	Block *b = c->getBlock(x-X*CHUNK_SIZE, y, z-Z*CHUNK_SIZE);
	if (b)
		b->unselect_block();
}


void World::put_block(int x, int y, int z, BlockType type)
{
	int X = chunked(x), Z = chunked(z);
	Chunk *c = findChunk(X, Z);
	if (!c) return;

	c->putBlock(x-X*CHUNK_SIZE, y, z-Z*CHUNK_SIZE, type);
}

void World::clear_block(int x, int y, int z)
{
	int X = chunked(x), Z = chunked(z);
	Chunk *c = findChunk(X, Z);
	if (!c)
		return;

	c->removeBlock(x-X*CHUNK_SIZE, y, z-Z*CHUNK_SIZE);
}

void World::init()
{
	// 初始化立方体渲染器
	cubeRender = new CubeRender();

	// 初始化天空盒
	skyBox.init(ResourceManager::GetShader("shader_skybox"));
}

void World::render()
{
	// 渲染天空盒
	skyBox.render();

	// 设置简单的光照
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	GLfloat gray[] = { 0.4, 0.4, 0.4, 1.0 };
	GLfloat light_pos[] = { 10, 10, 10, 1 };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, gray);
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT, gray);
	glEnable(GL_LIGHT0);

	for (auto &chunk : this->chunks) {
		chunk->render(cubeRender);
	}

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
}