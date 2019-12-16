#include "goldBlock.h"
#include "resource_manager.h"

GoldBlcok::GoldBlcok(float x, float y, float z) :Block(x, y, z)
{

}


void GoldBlcok::Draw(CubeRender* Render)
{
	Render->drawCube(position[0], position[1], position[2],
		ResourceManager::GetTexture("gold_ore"),
		ResourceManager::GetTexture("gold_ore"),
		ResourceManager::GetTexture("gold_ore"));

	if (isSelected)
	{
		Render->drawWireCube(position[0], position[1], position[2]);
	}
}