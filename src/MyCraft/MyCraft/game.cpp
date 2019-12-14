#include "game.h"
#include "resource_manager.h"

Game::Game(GLuint width, GLuint height)
{
	this->Width = width;
	this->Height = height;

	camera = nullptr;

	world = nullptr;

	mousePicker = nullptr;

	for (int i = 0; i < 1024; i++)
	{
		Keys[i] = false;
	}
	State = GAME_ACTIVE;
}

Game::~Game()
{
	if (camera != nullptr)
		delete camera;
	if (world != nullptr)
		delete world;
}

void Game::Init()
{
	// 创建一个照相机
	this->camera = new Camera(glm::vec3(3.0f, 8.0f, 3.0f));


	// 暂时先放在这里设置投影矩阵，因为要传给 mousePicker 做计算


	// float projectionMatrix[16];
	// glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrix);
	// 创建鼠标拾取器
	// this->mousePicker = new MousePicker(this->camera, glm::make_mat4(projectionMatrix));
	this->mousePicker = new MousePicker(this->camera, glm::perspective(65.0f, (float)Width / Height, 0.125f, 100.0f));

	// 载入游戏所需的着色器与纹理资源
	ResourceManager::LoadTexture("textures/grass_block_side.png", false, "tex_side");
	ResourceManager::LoadTexture("textures/Test.png", false, "tex_up");
	ResourceManager::LoadShader("shaders/sky_vertex.glsl", "shaders/sky_fragment.glsl", nullptr, "shader_skybox");

	// 新建一个 world 对象
	this->world = new World();
	// 载入世界资源
	this->world->Load();
	// 初始化世界
	this->world->init();

}

void Game::ProcessInput(GLfloat dt)
{
	dt = 0.01;

	camera->doMovement(this->Keys, world, dt);

	/*
		// 根据用户输入控制照相机的位置
	if (Keys['w'])
		camera->ProcessKeyboard(FORWARD, this->world ,dt);
	if (Keys['s'])
		camera->ProcessKeyboard(BACKWARD, this->world, dt);
	if (Keys['a'])
		camera->ProcessKeyboard(LEFT, this->world, dt);
	if (Keys['d'])
		camera->ProcessKeyboard(RIGHT, this->world, dt);
	if (Keys[' '])
		camera->ProcessKeyboard(JUMP, this->world, dt);
	camera->ProcessKeyboard(IDLE, this->world, dt);
	
	*/

}


void Game::Render()
{

	// 暂时使用固定管线渲染的方式，但是 skyBox 是用着色器渲染的

	// clear buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(65.0f, (float)Width / Height, 0.125f, 100.0f);

	// 设置模型矩阵
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// 设置视点
	camera->setLookAt();

	// 渲染世界
	world->render();

	// glutWireCube(10);

	// 测试鼠标射线
	this->mousePicker->render_ray();

	// 渲染屏幕中心的十字
	this->render_cross();

}

void Game::Update(GLfloat dt)
{
	// 会判断是否碰撞更新y坐标的位置，会判断是否跳跃
	// camera->updateCamera(this->world, dt);
}

// 先放这儿, 保存上一次选中的方块位置，为了能够不选中的时候消除线框
static int block_last_x = 0;	
static int block_last_y = 0;
static int block_last_z = 0;

void Game::MouseMoveCallback(int xpos, int ypos)
{
	this->camera->camera_mouse_callback(xpos, ypos);

	// update mouse picker
	this->mousePicker->update(xpos, ypos, Width, Height);


	// ---------------------------------------------------------
	// TODO: 测试射线拾取
	int distance = 8;			// 能够得着多远

	glm::vec3 ray = this->mousePicker->getCurrentRay();
	int x, y, z;
	for (int u = 0; u < distance; u++) {
		x = roundf(ray.x * u + camera->Position.x);
		y = roundf(ray.y * u + camera->Position.y);
		z = roundf(ray.z * u + camera->Position.z);
		if (world->get_map(x, y, z) ) {
			if (block_last_x == x && block_last_y == y && block_last_z == z) {
				// cout << "已经选中了"<< endl;
			}
			else {
				world->unpick_block(block_last_x, block_last_y, block_last_z);
				world->pick_block(x, y, z);
				block_last_x = x;
				block_last_y = y;
				block_last_z = z;
			}
			break;
		}
	}
	// --------------------------------------------
}

void Game::MouseClickCallback(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			// 下面是放置方块功能
			// 有了当前选中的方块，判断射线先与方块的哪一个面相交
			// cout << block_last_x << " " << block_last_y << " " << block_last_z << endl;
			glm::vec3 ray = this->mousePicker->getCurrentRay();
			glm::vec3 pos = glm::vec3(block_last_x, block_last_y, block_last_z);
			// 获取相交的面
			glm::vec3 p = this->mousePicker->Intersect(ray, camera->Position, block_last_x, block_last_y, block_last_z);
			if (p == glm::vec3(0, 0, 0)) {
				return;
			}
			pos = pos + p;

			if (!world->get_map(pos.x, pos.y, pos.z)) {	// 当前位置没有方块
				if (glm::abs(camera->Position.x - pos.x) < 1
					&& glm::abs(camera->Position.z - pos.z) < 1
					&& pos.y + 0.5 > camera->Position.y - 1.5
					&& pos.y - 0.5 < camera->Position.y + 0.5) {	// 要放的方块不能与人碰撞
					return;
				}
				// cout << "放在了:" << pos.x << " " << pos.y << " " << pos.z << endl;
				world->put_block(pos.x, pos.y, pos.z);
			}
		}
	}

}


void Game::render_cross()
{
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);  // 选择投影矩阵
	glPushMatrix();               // 保存原矩阵
	glLoadIdentity();             // 装入单位矩阵
	glOrtho(0, 100, 0, 100, -1, 1);    // 位置正投影
	glMatrixMode(GL_MODELVIEW);   // 选择Modelview矩阵
	glPushMatrix();               // 保存原矩阵
	glLoadIdentity();             // 装入单位矩阵
	glColor3f(0.0, 0.0, 0.0);
	glLineWidth(2.0f);
	glBegin(GL_LINES);

	glVertex2i(49, 50);
	glVertex2i(51, 50);
	glVertex2i(50, 49);
	glVertex2i(50, 51);
	glEnd();

	glMatrixMode(GL_PROJECTION);  // 选择投影矩阵
	glPopMatrix();                // 重置为原保存矩阵
	glMatrixMode(GL_MODELVIEW);   // 选择Modelview矩阵
	glPopMatrix();                // 重置为原保存矩阵
	glEnable(GL_DEPTH_TEST);
}