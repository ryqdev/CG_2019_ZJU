#include "grassBlock.h"
#include "resource_manager.h"

GrassBlcok::GrassBlcok(float x, float y, float z):Block(x, y, z)
{

}


void GrassBlcok::Draw(CubeRender* Render)
{
	Render->drawCube(position[0], position[1], position[2],
		ResourceManager::GetTexture("grass_block_up"),
		ResourceManager::GetTexture("grass_block_side"),
		ResourceManager::GetTexture("grass_block_up"));

	if (isSelected)
	{
		Render->drawWireCube(position[0], position[1], position[2]);
	}
}