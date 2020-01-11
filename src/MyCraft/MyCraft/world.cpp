#include "world.h"
#include "resource_manager.h"
#include "light.h"
#include <glm\gtx\string_cast.hpp>
#define Sin(x)  sin(x* 3.1415926 /180.0)
#define Cos(x)  cos(x * 3.1415926 / 180.0)
//DirectionLight light(GL_LIGHT0);
//PointLight light1(GL_LIGHT1);
//PointLight light2(GL_LIGHT2);
//SpotLight light3(GL_LIGHT3);
glm::vec3 light(1.0, 0.8, 0.8);//灯光的信息
glm::vec3 lightpos(20, 20, 20);//灯光位置
glm::vec3 ambient, diffuse, specular;//材质的信息
float shiness = 32.0f;//灯光的参数
glm::vec3 treepos[N_TREE];
World::World()
{
	//memset(mAmbientMaterial, 0, sizeof(mAmbientMaterial));
	//memset(mDiffuseMaterial, 0, sizeof(mDiffuseMaterial));
	//memset(mSpecularMaterial, 0, sizeof(mSpecularMaterial));
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

ChunkManager& World::getChunkManager()
{
	return chunkManager;
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
	int X = chunkManager.chunked(x), Z = chunkManager.chunked(z);
	Chunk* c = findChunk(X, Z);
	if (!c) return AIR;

	return c->getBlock(x - X * CHUNK_SIZE, y, z - Z * CHUNK_SIZE);
}

void World::put_block(int x, int y, int z, BlockType type)
{
	int X = chunkManager.chunked(x), Z = chunkManager.chunked(z);
	Chunk* c = findChunk(X, Z);
	if (!c) return;

	c->putBlock(x - X * CHUNK_SIZE, y, z - Z * CHUNK_SIZE, type);
}

void World::remove_block(int x, int y, int z)
{
	int X = chunkManager.chunked(x), Z = chunkManager.chunked(z);
	Chunk* c = findChunk(X, Z);
	if (!c) return;

	c->removeBlock(x - X * CHUNK_SIZE, y, z - Z * CHUNK_SIZE);
}

int World::highest(int x, int z)
{
	int X = chunkManager.chunked(x), Z = chunkManager.chunked(z);
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
	cubeRender = new CubeRender();

	// 初始化树木渲染器
//	treeRender = new TreeRender[N_TREE]();

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
	//初始化树
		//树木位置
	float treelist[N_TREE][2];

	for (int i = 0; i < N_TREE; i++) {
		treelist[i][0] = 100 - rand() % 200;
		treelist[i][1] = 100 - rand() % 200;
		//树的位置
		treepos[i] = glm::vec3(treelist[i][0], highest(treelist[i][0], treelist[i][1]), treelist[i][1]);
	}

	treeRender = new TreeRender();
	Tree tree;
	treeRender->initRenderData(tree);


	//// 初始化区块
	//chunkManager.init(ResourceManager::GetShader("shader_chunk"));
	//for (int i = -2; i <= 2; i++) {
	//	for (int j = -2; j <= 2; j++) {
	//		chunkManager.getChunk(i, j);
	//	}
	//}

	//设置光线参数
	//light.SetAmbientColor(0.6f, 0.6f, 0.6f, 1.0f);
	//light.SetDiffuseColor(0.8f, 0.8f, 0.8f, 1.0f);
	//light.SetSpecularColor(0.1f, 0.1f, 0.1f, 1.0f);
	//light.SetPosition(1.0, -1.0, -1.0f);

	//light1.SetAmbientColor(0.1f, 0.1f, 0.1f, 1.0f);
	//light1.SetDiffuseColor(0.2f, 1.0f, 0.6f, 1.0f);
	//light1.SetSpecularColor(0.1f, 0.1f, 0.1f, 1.0f);
	//light1.SetPosition(0.0f, 1.0f, 0.0f);
	//light1.SetConstAttenuation(1.0f);
	//light1.SetLinearAttenuation(0.2f);

	//light2.SetAmbientColor(0.1f, 0.1f, 0.1f, 1.0f);
	//light2.SetDiffuseColor(0.1f, 0.4f, 0.6f, 1.0f);
	//light2.SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	//light2.SetPosition(0.0f, 0.0f, -30.0f);
	//light2.SetConstAttenuation(1.0f);
	//light2.SetLinearAttenuation(0.2f);

	//light3.SetAmbientColor(0.1f, 0.1f, 0.1f, 1.0f);
	//light3.SetDiffuseColor(0.1f, 0.4f, 0.6f, 1.0f);
	//light3.SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	//light3.SetPosition(0.0f, 50.0f, 0.0f);
	//light3.SetDirection(0.0f, -1.0f, 0.0f);
	//light3.SetExponent(5.0f);
	//light3.SetCutoff(10.0f);

	//SetAmbientMaterial(0.6f, 0.6f, 0.6f, 1.0f);
	//SetDiffuseMaterial(0.4f, 0.4f, 0.4f, 1.0f);
	//SetSpecularMaterial(0.1f, 0.1f, 0.1f, 1.0f);
}

//void World::SetAmbientMaterial(float r, float g, float b, float a) {
//	mAmbientMaterial[0] = r;
//	mAmbientMaterial[1] = g;
//	mAmbientMaterial[2] = b;
//	mAmbientMaterial[3] = a;
//}
//
//void World::SetDiffuseMaterial(float r, float g, float b, float a) {
//	mDiffuseMaterial[0] = r;
//	mDiffuseMaterial[1] = g;
//	mDiffuseMaterial[2] = b;
//	mDiffuseMaterial[3] = a;
//}
//
//void World::SetSpecularMaterial(float r, float g, float b, float a) {
//	mSpecularMaterial[0] = r;
//	mSpecularMaterial[1] = g;
//	mSpecularMaterial[2] = b;
//	mSpecularMaterial[3] = a;
//}

//void World::render(Camera& camera)
//void World::render(glm::mat4 matrix, glm::vec3 cameraPos)
float ra = 0;
void World::render(glm::mat4 projection, glm::mat4 view, glm::vec3 cameraPos)
{
	// 渲染天空盒
	skyBox.render();

	lightpos.x = 20 * Sin(ra);
	lightpos.y = 20;
	lightpos.z = 20 * Cos(ra);
	//light.x = 0.3 + Sin(ra / 2);
	//light.y = 0.3 + Sin(ra / 2);
	//light.z = 0.3 + Sin(ra / 2);
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
	for (int i = 0; i < N_ROBOT; i++) {
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
	}
	glUseProgram(0);
	glEnable(GL_CULL_FACE);


	// 删除位于删除半径外的区块
	int cX = chunkManager.chunked(cameraPos.x);
	int cZ = chunkManager.chunked(cameraPos.z);
	for (auto it = chunks.begin(); it != chunks.end(); ) {
		if (max(abs((*it)->X - cX), abs((*it)->Z - cZ)) > DESTROY_RADIUS) {
			delete (*it);
			it = chunks.erase(it);
		}
		else {
			it++;
		}
	}




	// 更新并渲染区块
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	chunkshader.Use();

	glm::mat4 model = glm::mat4(1.0);
	chunkshader.SetMatrix4("project", projection);
	chunkshader.SetMatrix4("view", view);
	chunkshader.SetMatrix4("model", model);

	chunkshader.SetVector3f("lightcolor", light);
	chunkshader.SetVector3f("lightpos", lightpos);
	chunkshader.SetFloat("shiness", shiness);
	ambient = glm::vec3(0.45, 0.55, 0.65);
	diffuse = glm::vec3(0.65, 0.85, 0.65);
	specular = glm::vec3(1, 1, 1);
	chunkshader.SetVector3f("ambient", ambient);
	chunkshader.SetVector3f("diffuse", diffuse);
	chunkshader.SetVector3f("specular", specular);
	chunkshader.SetVector3f("viewpos", cameraPos);
	//std::cout << glm::to_string(cameraPos) << std::endl;
	Texture2DArray textureArray = ResourceManager::GetTextureArray("blocks");
	textureArray.Bind();

	for (int i = -CREATE_RADIUS; i <= CREATE_RADIUS; i++) {
		for (int j = -CREATE_RADIUS; j <= CREATE_RADIUS; j++) {
			Chunk* c = findChunk(cX + i, cZ + j);
			// 如果指定位置不存在区块，则创建或载入一个区块
			if (!c) {
				c = new Chunk(cX + i, cZ + j);
				chunks.push_back(c);
			}
			c->render();
		}
	}
	//渲染树木 最多N_TREE数量树木
	sence.Use();
	sence.SetVector3f("lightcolor", light);
	sence.SetVector3f("lightpos", lightpos);
	sence.SetFloat("shiness", shiness);

	sence.SetVector3f("viewpos", cameraPos);
	model = glm::mat4(1.0);
	sence.SetMatrix4("project", projection);

	ambient = glm::vec3(1, 0, 0);
	diffuse = glm::vec3(1, 0, 0);
	specular = glm::vec3(1, 1, 1);
	sence.SetVector3f("ambient", ambient);
	sence.SetVector3f("diffuse", diffuse);
	sence.SetVector3f("specular", specular);
	sence.SetMatrix4("view", view);
	{
		sun.Bind();
		model = glm::translate(model, glm::vec3(glm::vec3(lightpos.x, lightpos.y, lightpos.z)));
		sence.SetMatrix4("model", model);
		robotRender->DrawBall();
	}


	ambient = glm::vec3(0.5, 1, .2351);
	diffuse = glm::vec3(0.5, 1, 0.251);
	specular = glm::vec3(0, 1, 0);
	sence.SetVector3f("ambient", ambient);
	sence.SetVector3f("diffuse", diffuse);
	sence.SetVector3f("specular", specular);
	for (int i = 0; i < N_TREE; i++) {
		model = glm::mat4(1.0);
		//	model = glm::scale(model, glm::vec3(10, 10, 10));
		model = glm::translate(model, glm::vec3(treepos[i].x, treepos[i].y, treepos[i].z));

		//	model = glm::translate(model, glm::vec3(glm::vec3(lightpos.x, lightpos.y, lightpos.z)));
		sence.SetMatrix4("model", model);

		//	std::cout << glm::to_string(model) << std::endl;
		treeRender->DrawTree(sence, trunk, leaves);
	}



	//	glBindTexture(GL_TEXTURE_2D, 0);
		//glEnable(GL_CULL_FACE);



	if (picked) {
		glm::mat4 matrix = projection * view;
		cubeRender->drawWireCube(pickedBlock.x, pickedBlock.y, pickedBlock.z, matrix);
	}

	glUseProgram(0);




	// 设置简单的光照
	//设置光照
	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_LIGHTING);
	//glMaterialfv(GL_FRONT, GL_AMBIENT, mAmbientMaterial);
	//glMaterialfv(GL_FRONT, GL_DIFFUSE, mDiffuseMaterial);
	//glMaterialfv(GL_FRONT, GL_SPECULAR, mSpecularMaterial);
	//glEnable(GL_COLOR_MATERIAL);
	//light.Enable();
	//light1.Enable();
	//light2.Enable();
	//light3.Enable();

	//// 设置简单的光照
	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_LIGHTING);
	//GLfloat gray[] = { 0.4, 0.4, 0.4, 1.0 };
	//GLfloat light_pos[] = { 10, 10, 10, 1 };
	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, gray);
	//glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
	//glLightfv(GL_LIGHT0, GL_AMBIENT, gray);
	//glEnable(GL_LIGHT0);
	/*
	if (picked) {
		cubeRender->drawWireCube(pickedBlock.x, pickedBlock.y, pickedBlock.z);
	}*/

	//	glDisable(GL_LIGHTING);
		//glDisable(GL_DEPTH_TEST);
}
