#include "world.h"
#include "resource_manager.h"
#include "light.h"
#include <glm\gtx\string_cast.hpp>
#define Sin(x)  sin(x* 3.1415926 /180.0)
#define Cos(x)  cos(x * 3.1415926 / 180.0)
glm::vec3 light(1.0, 0.8, 0.8);//灯光的信息
glm::vec3 lightpos(20, 20, 20);//灯光位置
glm::vec3 ambient, diffuse, specular;//材质的信息
float shiness = 32.0f;//灯光的参数
//glm::vec3 treepos[N_TREE];
vector<glm::vec3> treepos;
World::World()
{
}

int World::chunked(int x)
{
	return floor(float(x) / CHUNK_SIZE);
}

Chunk* World::findChunk(int x, int z)
{
	for (auto it = chunks.begin(); it != chunks.end(); it++) {
		if ((*it)->X == x && (*it)->Z == z)
			return (*it);
	}
	return nullptr;
}

World::~World()
{
	// 释放渲染器
	if (cubeRender != nullptr)
		delete cubeRender;
	//释放树木渲染器
	if (treeRender != nullptr)
		delete treeRender;
}

void World::pick_block(int x, int y, int z)
{
	picked = true;
	pickedBlock = { x, y, z };
}

void World::unpick_block()
{
	picked = false;
}

BlockType World::get_block(int x, int y, int z)
{
	int X = chunked(x), Z = chunked(z);
	Chunk* c = findChunk(X, Z);
	if (!c) return AIR;

	return c->getBlock(x - X * CHUNK_SIZE, y, z - Z * CHUNK_SIZE);
}

void World::put_block(int x, int y, int z, BlockType type)
{
	int X = chunked(x), Z = chunked(z);
	Chunk* c = findChunk(X, Z);
	if (!c) return;

	c->putBlock(x - X * CHUNK_SIZE, y, z - Z * CHUNK_SIZE, type);
}

void World::remove_block(int x, int y, int z)
{
	int X = chunked(x), Z = chunked(z);
	Chunk* c = findChunk(X, Z);
	if (!c) return;

	c->removeBlock(x - X * CHUNK_SIZE, y, z - Z * CHUNK_SIZE);
}

int World::highest(int x, int z)
{
	int X = chunked(x), Z = chunked(z);
	Chunk* c = findChunk(X, Z);
	if (!c) return 0;

	return c->highest(x - X * CHUNK_SIZE, z - Z * CHUNK_SIZE);
}

void World::Load()
{
	for (int i = -2; i <= 2; i++) {
		for (int j = -2; j <= 2; j++) {
			Chunk* chunk = new Chunk(i, j);
			chunk->genChunk();
			chunk->genBuffer();
			chunks.push_back(chunk);
		}
	}

	this->skyBox = SkyBox();
}

void World::init()
{
	// 初始化立方体渲染器
	Shader lineShader = ResourceManager::GetShader("shader_line");
	cubeRender = new CubeRender(lineShader);

	// 初始化机器人
	robotRender = new RobotRender();

	// 初始化天空盒
	skyBox.init(ResourceManager::GetShader("shader_skybox"));

	sence = ResourceManager::GetShader("shader_sence");

	trunk = ResourceManager::GetTexture("trunk");
	leaves = ResourceManager::GetTexture("leaves");

	sun = ResourceManager::GetTexture("sun");
	earth = ResourceManager::GetTexture("earth");
	moon = ResourceManager::GetTexture("moon");
	chunkshader = ResourceManager::GetShader("shader_chunk");

	// 初始化区块
	const int CREATE_RADIUS = 5;
	for (int i = -CREATE_RADIUS; i <= CREATE_RADIUS; i++) {
		for (int j = -CREATE_RADIUS; j <= CREATE_RADIUS; j++) {
			Chunk* c = findChunk(i, j);
			// 如果指定位置不存在区块，则创建或载入一个区块
			if (!c) {
				c = new Chunk(i, j);
				chunks.push_back(c);
			}
		}
	}

	//初始化树
	//树木位置
	int mapSize = (CREATE_RADIUS * 2 + 1) * CHUNK_SIZE;
	int treeNum = (CREATE_RADIUS * 2 + 1) * (CREATE_RADIUS * 2 + 1) * 4;
	for (int i = 0; i < treeNum; i++) {
		int x = mapSize/2 - rand() % mapSize;
		int z = mapSize/2 - rand() % mapSize;
		int y = highest(x, z);

		// 假设树的碰撞体积为3个方块
		put_block(x, y + 1, z, TREE);
		put_block(x, y + 2, z, TREE);
		put_block(x, y + 3, z, TREE);

		// 树的位置
		treepos.push_back({x, y + 0.5, z});
	}

	// 初始化树木渲染器
	treeRender = new TreeRender();
	Tree tree;
	treeRender->initRenderData(tree);
}

float ra = 0;
void World::render(glm::mat4 projection, glm::mat4 view, glm::vec3 cameraPos)
{
	// 渲染天空盒
	skyBox.render();

	lightpos.x = 20 * Sin(ra);
	lightpos.y = 20;
	lightpos.z = 20 * Cos(ra);
	ra += 1;
	if (ra > 360)
		ra -= 360;
	

	sence.Use();
	//Shader r = ResourceManager::GetShader("shader_robot");
	//r.Use();
	//r.SetMatrix4("project", projection);
	//r.SetMatrix4("view", view);

	ambient = glm::vec3(0.7895, 0.8, 0.88);
	diffuse = glm::vec3(0.2, 1, 0.5);
	specular = glm::vec3(1, 1, 1);
	sence.SetVector3f("ambient", ambient);
	sence.SetVector3f("diffuse", diffuse);
	sence.SetVector3f("specular", specular);
	
	//渲染robot
	/*for (int i = 0; i < N_ROBOT; i++) {
		robotRender->robotList[i].randomMove();
		float x = robotRender->robotList[i].x;
		float z = robotRender->robotList[i].z;
		robotRender->robotList[i].setLocation(x, highest(x, z), z);
		Robot robot = robotRender->robotList[i];

		robotRender->DrawRobot(sence, robot, earth, moon);



		//glm::mat4 model(1.0);
		//model = glm::translate(model, glm::vec3(robot.x, robot.y, robot.z));
		//model = glm::rotate(model, robot.vangle, glm::vec3(0, 1, 0));
		//model = glm::scale(model, glm::vec3(robot.size, robot.size, robot.size));
		//std::cout << robot.size << std::endl;
		//	std::cout << glm::to_string(model) << std::endl;
	//	r.SetMatrix4("model", model);
		//	glCallList(lid);

	//	glBindVertexArray(robotRender->vao);
	//	glDrawArrays(GL_TRIANGLES, 0, robotRender->count);
	//	glBindVertexArray(0);

		//robotRender->DrawRobot(r, robotRender->robotList[i]);
	}*/
	glUseProgram(0);
	glEnable(GL_CULL_FACE);

	// 渲染区块
	chunkshader.Use();
	chunkshader.SetMatrix4("project", projection);
	chunkshader.SetMatrix4("view", view);
	chunkshader.SetMatrix4("model", glm::mat4(1.0));
	chunkshader.SetFloat("shiness", shiness);
	chunkshader.SetVector3f("lightPos", lightpos);
	chunkshader.SetVector3f("viewpos", cameraPos);
	chunkshader.SetVector3f("light.ambient", 0.2f, 0.2f, 0.2f);
	chunkshader.SetVector3f("light.diffuse", 0.6f, 0.6f, 0.6f);
	chunkshader.SetVector3f("light.specular", 0.85f, 0.85f, 0.85f);
	Texture2DArray textureArray = ResourceManager::GetTextureArray("blocks");
	textureArray.Bind();

	for (Chunk *c : chunks) {
		c->render();
	}

	//渲染树木 最多N_TREE数量树木
	sence.Use();
	sence.SetVector3f("lightcolor", light);
	sence.SetVector3f("lightpos", lightpos);
	sence.SetFloat("shiness", shiness);

	sence.SetVector3f("viewpos", cameraPos);
	sence.SetMatrix4("project", projection);

	ambient = glm::vec3(1, 0, 0);
	diffuse = glm::vec3(1, 0, 0);
	specular = glm::vec3(1, 1, 1);
	sence.SetVector3f("ambient", ambient);
	sence.SetVector3f("diffuse", diffuse);
	sence.SetVector3f("specular", specular);
	sence.SetMatrix4("view", view);
	sun.Bind();
	sence.SetMatrix4("model", glm::translate(glm::mat4(1.0), lightpos));
	robotRender->DrawBall();
	
	ambient = glm::vec3(0.5, 1, .2351);
	diffuse = glm::vec3(0.5, 1, 0.251);
	specular = glm::vec3(0, 1, 0);
	sence.SetVector3f("ambient", ambient);
	sence.SetVector3f("diffuse", diffuse);
	sence.SetVector3f("specular", specular);

	for (auto pos : treepos) {
		glm::mat4 model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(pos.x, pos.y, pos.z));
		sence.SetMatrix4("model", model);
		treeRender->DrawTree(sence, trunk, leaves);
	}

	if (picked) {
		glm::mat4 model = glm::mat4(1.0);
		model = glm::translate(model, {pickedBlock.x, pickedBlock.y, pickedBlock.z});
		glm::mat4 matrix = projection * view * model;
		cubeRender->drawWireCube(matrix);
	}

	glUseProgram(0);
}
