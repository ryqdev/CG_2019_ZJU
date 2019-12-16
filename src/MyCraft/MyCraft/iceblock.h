#pragma once
#include "block.h"

class IceBlock :public Block
{
public:
	IceBlock(float x, float y, float z);
	void Draw(CubeRender* Render);
};