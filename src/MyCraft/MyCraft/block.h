#pragma once

#include "cubeRender.h"
#include <vector>

class Block
{
private:
	bool isSelected;


public:
	// 方块在世界中的坐标, (方块的中心点)
	std::vector<float> position;

	Block(float x, float y, float z);

	Block();

	// 绘制方块
	void Draw(CubeRender* Render);

	void select_block();
	void unselect_block();
};