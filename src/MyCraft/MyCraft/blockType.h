#pragma once

static int BlockTypeNum = 3; // 方块的种类数

enum BlockType {
	GRASS,
	ICE,
	GOLD,
	TREE = 254,	// 放到后面，只起到标志的作用
	AIR = 255	// 放到最后
};

namespace ETextureType {
	enum TextureType {
		GRASS_TOP,
		GRASS_SIDE,
		ICE,
		GOLD_ORE
	};

	// 0~6分别是left, right, top, bottom, front, back
	const int blockTextures[][6] = {
		{GRASS_SIDE, GRASS_SIDE, GRASS_TOP, GRASS_TOP, GRASS_SIDE, GRASS_SIDE},	// GRASS
	{ICE, ICE, ICE, ICE, ICE, ICE},	// ICE
	{GOLD_ORE, GOLD_ORE, GOLD_ORE, GOLD_ORE, GOLD_ORE, GOLD_ORE},	// GOLD
	};
}


