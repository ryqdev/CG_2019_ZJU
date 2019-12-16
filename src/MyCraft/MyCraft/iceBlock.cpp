#include "iceblock.h"
#include "resource_manager.h"

IceBlock::IceBlock(float x, float y, float z) :Block(x, y, z)
{

}
void IceBlock::Draw(CubeRender* Render)
{
	Render->drawCube(position[0], position[1], position[2],
		ResourceManager::GetTexture("ice_block"),
		ResourceManager::GetTexture("ice_block"),
		ResourceManager::GetTexture("ice_block"));

	if (isSelected)
	{
		Render->drawWireCube(position[0], position[1], position[2]);
	}
}