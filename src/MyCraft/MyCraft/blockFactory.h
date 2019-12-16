#pragma once
#include "block.h"
#include "grassBlock.h"
#include "iceblock.h"
#include "blockType.h"


static class BlockFactory
{
public:
	static Block* getBlock(int x, int y, int z, BlockType type)
	{
		switch (type)
		{
		case GRASS:
			return new GrassBlcok(x, y, z);
			break;
		case ICE:
			return new IceBlock(x, y, z);
			break;
		default:
			break;
		}
	}
};