#include "block.h"
#include "resource_manager.h"

Block::Block(float x, float y, float z)
{
	this->position.push_back(x);
	this->position.push_back(y);
	this->position.push_back(z);
	this->isSelected = false;
}

Block::Block()
{
	this->position.push_back(0);
	this->position.push_back(0);
	this->position.push_back(0);
	this->isSelected = false;
}


void Block::Draw(CubeRender* Render)
{
	Render->drawCube(position[0], position[1], position[2], 
		ResourceManager::GetTexture("tex_up"),
		ResourceManager::GetTexture("tex_side"),
		ResourceManager::GetTexture("tex_up"));
	
	if (isSelected) 
	{
		Render->drawWireCube(position[0], position[1], position[2]);
	}
}

void Block::select_block()
{
	isSelected = true;
}

void Block::unselect_block()
{
	isSelected = false;
}