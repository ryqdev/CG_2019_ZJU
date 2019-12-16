#include "world.h"
#include "resource_manager.h"


World::World()
{
	for (int i = 0; i < WIDTH; i++) {
		for (int j = 0; j < WIDTH; j++) {
			for (int k = 0; k < WIDTH; k++) {
				map[i][j][k] = 0;
			}
		}
	}
}

char World::get_map(int x, int y, int z)
{
	if (x < -WIDTH / 2 || x > WIDTH / 2 || y < -WIDTH / 2 || y > WIDTH / 2 || z < -WIDTH / 2 || z > WIDTH / 2)
		return 0;
	return this->map[x + WIDTH / 2][y + WIDTH / 2][z + WIDTH / 2];
}

void World::set_map(int x, int y, int z)
{
	map[x + WIDTH / 2][y + WIDTH / 2][z + WIDTH / 2] = 1;
}

void World::reset_map(int x, int y, int z)
{
	map[x + WIDTH / 2][y + WIDTH / 2][z + WIDTH / 2] = 0;
}



World::~World()
{
	// 释放渲染器
	if (cubeRender != nullptr)
		delete cubeRender;
}

void World::Load()
{
	// 暂时先这样吧
	for (int i = -5; i < 5; i++)
	{
		for (int j = -5; j < 5; j++)
		{
			this->put_block(i, 0, j, GRASS);
		}
	}


	this->put_block(0, 1, 0, GRASS);	// 中间那个方块

	
	this->put_block(-5, 1, -5, GRASS);	// 四周一圈
	this->put_block(-5, 1, -4, GRASS);
	this->put_block(-5, 1, -3, GRASS);
	this->put_block(-5, 1, -2, GRASS);
	this->put_block(-5, 1, -1, GRASS);
	this->put_block(-5, 1, -0, GRASS);
	this->put_block(-5, 1, 1, GRASS);	// 四周一圈
	this->put_block(-5, 1, 2, GRASS);
	this->put_block(-5, 1, 3, GRASS);
	this->put_block(-5, 1, 4, GRASS);
	this->put_block(-5, 1, 5, GRASS);

	this->skyBox = SkyBox();
}

int World::get_block_id(int x, int y, int z)
{
	// 暂时先这样，是否会溢出?
	return y * WIDTH * WIDTH + x * WIDTH + z;
}

void World::pick_block(int x, int y, int z)
{
	int id = get_block_id(x, y, z);

	std::unordered_map<int, Block*>::iterator it;

	it = this->blockMap.find(id);

	if (it != this->blockMap.end())
	{
		it->second->select_block();
	}
}

void World::unpick_block(int x, int y, int z)
{
	int id = get_block_id(x, y, z);
	std::unordered_map<int, Block*>::iterator it;

	it = this->blockMap.find(id);

	if (it != this->blockMap.end())
	{
		it->second->unselect_block();
	}
}


void World::put_block(int x, int y, int z, BlockType type)
{
	set_map(x, y, z);

	Block* block = BlockFactory::getBlock(x, y, z, type);
	
	// 注意映射方式, id 是否会溢出
	int id = get_block_id(x, y, z);
	blockMap.insert(std::pair<int, Block*>(id, block));
}

void World::clear_block(int x, int y, int z)
{
	reset_map(x, y, z);

	int id = get_block_id(x, y, z);
	std::unordered_map<int, Block*>::iterator it;

	it = this->blockMap.find(id);
	if (it != this->blockMap.end())
	{
		delete it->second;	// 释放指针
		it->second = nullptr;
		this->blockMap.erase(it);
	}
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

	for (auto& block : this->blockMap)
	{
		block.second->Draw(cubeRender);
	}

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
}