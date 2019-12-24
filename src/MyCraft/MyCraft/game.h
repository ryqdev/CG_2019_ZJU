#pragma once
#include "GL/glew.h"
#include "camera.h"
#include "world.h"
#include "mousePicker.h"

// Represents the current state of the game
enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};


class Game
{
private:
	BlockType currentType;	// 当前鼠标放置的方块类型

public:
	// 游戏状态
	GameState  State;
	GLboolean  Keys[1024];
	// 游戏窗口的宽高
	GLuint     Width, Height;

	Camera* camera;

	MousePicker* mousePicker;

	World* world;

	void nextBlcokType();

	// Constructor/Destructor
	Game(GLuint width, GLuint height);
	~Game();
	
	// 加载资源(着色器, 纹理), 初始化游戏状态
	void Init();

	// 使用存储在 keys 数组中的数据来处理用户输入
	void ProcessInput(GLfloat dt);

	void MouseMoveCallback(int x, int y);

	void MouseClickCallback(int button, int state, int x, int y);

	// 窗口改变时更新摄像机状态
	void Reshape(int w, int h);

	// 在该函数中更新游戏状态设置,例如动物的自己移动
	void Update(GLfloat dt);

	// 渲染游戏
	void Render();

private:

	void render_cross();

};