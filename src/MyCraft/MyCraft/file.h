//txt存储 xyztype

#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "blockType.h"
#include "GL/glew.h"
using namespace std;

class File
{
public:
	ofstream outfile;//写入txt操作
	vector<GLint>vx;//记录txt文件中立方体坐标及blocktype
	vector<GLint>vy;
	vector<GLint>vz;
	vector<GLint>vtype;

	string filename = "";
	string openfilename = "";

	bool ExistTxt(string pathName);//判断是否存在txt
	void CreateFolder(string folderPath);//创建save文件夹
	void Init();//创建txt文件，文件与流关联

	void WriteCube(int x, int y, int z, BlockType type);//增加方块时写入方块坐标及blocktype
	void RemoveCube(int x, int y, int z);//删去方块时写入方块坐标，BlockType对应列写入-1
	void ReadFile();//读取文件中所有增删方块操作，记入对应容器
};