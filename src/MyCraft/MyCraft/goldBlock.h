#pragma once
#include "block.h"

class GoldBlcok :public Block
{
public:
	GoldBlcok(float x, float y, float z);
	void Draw(CubeRender* Render);
};