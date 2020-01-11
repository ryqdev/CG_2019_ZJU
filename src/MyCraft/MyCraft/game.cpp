#include "game.h"
#include "resource_manager.h"

Game::Game(GLuint width, GLuint height)
{
	this->Width = width;
	this->Height = height;

	camera = nullptr;

	world = nullptr;

	mousePicker = nullptr;

	currentType = GRASS;

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

void Game::nextBlcokType()
{
	this->currentType = (BlockType)((this->currentType + 1) % BlockTypeNum);
	//cout << this->currentType << endl;
}


void Game::Init()
{
	// 纹理数组
	vector<const GLchar*> files;
	files.push_back("textures/grass_top.png");
	files.push_back("textures/grass_top_s.png");
	files.push_back("textures/grass_top_n.png");
	files.push_back("textures/grass_side.png");
	files.push_back("textures/grass_side_s.png");
	files.push_back("textures/grass_side_n.png");
	files.push_back("textures/ice.png");
	files.push_back("textures/ice_s.png");
	files.push_back("textures/ice_n.png");
	files.push_back("textures/gold_ore.png");
	files.push_back("textures/gold_ore_s.png");
	files.push_back("textures/gold_ore_n.png");
	ResourceManager::LoadTextureArray(files, false, "blocks");

	//装载树木纹理
	ResourceManager::LoadTexture("textures/tree.png", false, "trunk");
	ResourceManager::LoadTexture("textures/leaf.png", false, "leaves");
	ResourceManager::LoadTexture("textures/moon1024.bmp", false, "moon");
	ResourceManager::LoadTexture("textures/earth2048.bmp", false, "earth");
	ResourceManager::LoadTexture("textures/sunmap.bmp", false, "sun");

	ResourceManager::LoadShader("shaders/sky_vertex.glsl", "shaders/sky_fragment.glsl", nullptr, "shader_skybox");
	ResourceManager::LoadShader("shaders/chunk_vertex.glsl", "shaders/chunk_fragment.glsl",
		nullptr, "shader_chunk");
	ResourceManager::LoadShader("shaders/line_vertex.glsl", "shaders/line_fragment.glsl",
		nullptr, "shader_line");
	ResourceManager::LoadShader("shaders/robot_vertex.glsl", "shaders/robot_fragment.glsl", nullptr, "shader_robot");
	ResourceManager::LoadShader("shaders/sence_vertex.glsl", "shaders/sence_fragment.glsl", nullptr, "shader_sence");


	// 新建一个 world 对象
	this->world = new World();
	// 载入世界资源
	this->world->Load();
	// 初始化世界
	this->world->init();

	// 创建一个照相机
	this->camera = new Camera(glm::vec3(3.0f, this->world->highest(3, 3) + 2.5f, 3.0f));

	// 创建鼠标拾取器
	// this->mousePicker = new MousePicker(this->camera, glm::make_mat4(projectionMatrix));
	this->mousePicker = new MousePicker(this->camera, glm::perspective(65.0f, (float)Width / Height, 0.125f, 100.0f));

	currentType = GRASS;
}

void Game::ProcessInput(GLfloat dt)
{
	camera->doMovement(this->Keys, world, dt);
	camera->doZoom(this->Keys, dt);
}


void Game::Render()
{
	// clear buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(camera->getZooom(), (float)Width / Height, 0.125f, 100.0f);

	//设置模型矩阵
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//设置视点
	//camera->setLookAt();

	glm::mat4 view = camera->GetViewMatrix();
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)Width / Height, 0.125f, 100.0f);
	//	glm::mat4 matrix = projection * view;

		// 渲染世界
		//world->render(matrix, camera->Position);
	world->render(projection, view, camera->Position);
	// 测试鼠标射线
	this->mousePicker->render_ray();

	// 渲染屏幕中心的十字
	this->render_cross();

}

void Game::Update(GLfloat dt)
{
}

// 先放这儿, 保存上一次选中的方块位置，为了能够不选中的时候消除线框
static int block_last_x = 0;
static int block_last_y = 0;
static int block_last_z = 0;
static bool picked = false;

void Game::MouseMoveCallback(int xpos, int ypos)
{
	this->camera->camera_mouse_callback(xpos, ypos);

	// update mouse picker
	this->mousePicker->update(xpos, ypos, Width, Height);


	// ---------------------------------------------------------
	// TODO: 测试射线拾取
	int distance = 8;			// 能够得着多远
	bool isPicked = false;
	glm::vec3 ray = this->mousePicker->getCurrentRay();
	int x, y, z;
	for (int u = 0; u < distance; u++) {
		x = roundf(ray.x * u + camera->Position.x);
		y = roundf(ray.y * u + camera->Position.y);
		z = roundf(ray.z * u + camera->Position.z);
		if (Chunk::isBlock(world->get_block(x, y, z))) {
			if (block_last_x == x && block_last_y == y && block_last_z == z && picked) {
				// cout << "已经选中了"<< endl;
			}
			else {
				world->unpick_block();
				world->pick_block(x, y, z);
				block_last_x = x;
				block_last_y = y;
				block_last_z = z;
				picked = true;
			}
			isPicked = true;
			break;
		}
	}
	// 如果没有被选择的方块，则取消选择
	if (!isPicked) {
		world->unpick_block();
		picked = false;
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

			if (world->get_block(pos.x, pos.y, pos.z) == AIR) {	// 当前位置没有方块
				if (glm::abs(camera->Position.x - pos.x) < 0.6
					&& glm::abs(camera->Position.z - pos.z) < 0.6
					&& pos.y + 0.3 > camera->Position.y - 1.3
					&& pos.y - 0.3 < camera->Position.y + 0.3) {	// 要放的方块不能与人碰撞
					return;
				}
				world->put_block(pos.x, pos.y, pos.z, currentType);
				file.WriteCube(pos.x, pos.y, pos.z, currentType);
			}
		}
	}
	else if (button == GLUT_RIGHT_BUTTON) {		// 消除方块
		if (state == GLUT_DOWN) {
			if (Chunk::isBlock(world->get_block(block_last_x, block_last_y, block_last_z))) {
				world->remove_block(block_last_x, block_last_y, block_last_z);
				file.RemoveCube(block_last_x, block_last_y, block_last_z);
			}
		}
	}

}

void Game::Reshape(int w, int h)
{
	Width = w;
	Height = h;
	camera->setProjectionMatrix(glm::perspective(glm::radians(45.0f), (float)w / h, 0.125f, 100.0f));
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

void Game::ReDrawCube()
{
	for (int i = 0; i < file.vx.size(); i++) {
		int ix = file.vx[i], iy = file.vy[i], iz = file.vz[i];
		switch (file.vtype[i]) {
		case -1:world->remove_block(ix, iy, iz); break;
		case 0:world->put_block(ix, iy, iz, GRASS); break;
		case 1:world->put_block(ix, iy, iz, ICE); break;
		case 2:world->put_block(ix, iy, iz, GOLD); break;
		}
	}
}
