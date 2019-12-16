#pragma once
#include "block.h"

class GlodBlcok :public Block
{
public:
	GlodBlcok(float x, float y, float z);
	void Draw(CubeRender* Render);
};