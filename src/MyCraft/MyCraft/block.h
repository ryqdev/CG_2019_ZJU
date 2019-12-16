#pragma once

#include "cubeRender.h"
#include <vector>

class Block
{
protected:
	bool isSelected;


public:
	// 方块在世界中的坐标, (方块的中心点)
	std::vector<float> position;

	Block(float x, float y, float z);

	Block();

	// 绘制方块，虚函数，具体绘制由派生类决定
	virtual void Draw(CubeRender* Render) = 0;

	void select_block();
	void unselect_block();
};