#pragma once
#include "block.h"

class GrassBlcok :public Block 
{
public:
	GrassBlcok(float x, float y, float z);
	void Draw(CubeRender* Render);
};