#include "world.h"
#include "resource_manager.h"
#include "light.h"

int World::chunked(int x) {
	return floor(float(x) / CHUNK_SIZE);
}

DirectionLight light(GL_LIGHT0);
PointLight light1(GL_LIGHT1);
PointLight light2(GL_LIGHT2);
SpotLight light3(GL_LIGHT3);

World::World()
{
	memset(mAmbientMaterial, 0, sizeof(mAmbientMaterial));
	memset(mDiffuseMaterial, 0, sizeof(mDiffuseMaterial));
	memset(mSpecularMaterial, 0, sizeof(mSpecularMaterial));
}

Chunk* World::findChunk(int x, int z)
{
	for (auto it = chunks.begin(); it != chunks.end(); it++) {
		if ((*it)->X == x && (*it)->Z == z)
			return (*it);
	}
	return nullptr;
}

void World::drawWireCube(int x, int y, int z, glm::mat4 matrix)
{
	const float vertices[][3] = {
		-1, -1, -1,
		 1, -1, -1,
		 1,  1, -1,
		-1,  1, -1,
		-1, -1,  1,
		 1, -1,  1,
		 1,  1,  1,
		-1,  1,  1,
	};

	const int indices[] = {
		0, 1, 0, 3, 0, 4,
		1, 2, 1, 5, 2, 3,
		2, 6, 3, 7, 4, 5,
		4, 7, 5, 6, 6, 7
	};

	float data[24*3];
	for (int i = 0; i < 24; i++) {
		data[i*3] = x+vertices[indices[i]][0]*0.6;
		data[i*3+1] = y+vertices[indices[i]][1]*0.6;
		data[i*3+2] = z+vertices[indices[i]][2]*0.6;
	}

	Shader s = ResourceManager::GetShader("shader_line");
	s.Use();
	s.SetMatrix4("matrix", matrix, false);

	GLuint vao, vbo;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*3, (void *)0);

	glDrawArrays(GL_LINES, 0, 24);

	glBindVertexArray(0);

	glUseProgram(0);
}

World::~World()
{
	// 释放渲染器
	if (cubeRender != nullptr)
		delete cubeRender;
}

void World::pick_block(int x, int y, int z)
{
	picked = true;
	pickedBlock = {x, y, z};
}

void World::unpick_block()
{
	picked = false;
}

BlockType World::get_block(int x, int y, int z)
{
	int X = chunked(x), Z = chunked(z);
	Chunk *c = findChunk(X, Z);
	if (!c) return AIR;

	return c->getBlock(x-X*CHUNK_SIZE, y, z-Z*CHUNK_SIZE);
}

void World::put_block(int x, int y, int z, BlockType type)
{
	int X = chunked(x), Z = chunked(z);
	Chunk *c = findChunk(X, Z);
	if (!c) return;

	c->putBlock(x-X*CHUNK_SIZE, y, z-Z*CHUNK_SIZE, type);
}

void World::remove_block(int x, int y, int z)
{
	int X = chunked(x), Z = chunked(z);
	Chunk *c = findChunk(X, Z);
	if (!c) return;

	c->removeBlock(x-X*CHUNK_SIZE, y, z-Z*CHUNK_SIZE);
}

int World::highest(int x, int z)
{
	int X = chunked(x), Z = chunked(z);
	Chunk *c = findChunk(X, Z);
	if (!c) return 0;

	return c->highest(x-X*CHUNK_SIZE, z-Z*CHUNK_SIZE);
}

void World::Load()
{
	for (int i = -2; i <= 2; i++) {
		for (int j = -2; j <= 2; j++) {
			Chunk *chunk = new Chunk(i, j);
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

	// 初始化天空盒
	skyBox.init(ResourceManager::GetShader("shader_skybox"));
	
	//设置光线参数
	light.SetAmbientColor(0.6f, 0.6f, 0.6f, 1.0f);
	light.SetDiffuseColor(0.8f, 0.8f, 0.8f, 1.0f);
	light.SetSpecularColor(0.1f, 0.1f, 0.1f, 1.0f);
	light.SetPosition(1.0, -1.0, -1.0f);

	light1.SetAmbientColor(0.1f, 0.1f, 0.1f, 1.0f);
	light1.SetDiffuseColor(0.2f, 1.0f, 0.6f, 1.0f);
	light1.SetSpecularColor(0.1f, 0.1f, 0.1f, 1.0f);
	light1.SetPosition(0.0f, 1.0f, 0.0f);
	light1.SetConstAttenuation(1.0f);
	light1.SetLinearAttenuation(0.2f);

	light2.SetAmbientColor(0.1f, 0.1f, 0.1f, 1.0f);
	light2.SetDiffuseColor(0.1f, 0.4f, 0.6f, 1.0f);
	light2.SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	light2.SetPosition(0.0f, 0.0f, -30.0f);
	light2.SetConstAttenuation(1.0f);
	light2.SetLinearAttenuation(0.2f);

	light3.SetAmbientColor(0.1f, 0.1f, 0.1f, 1.0f);
	light3.SetDiffuseColor(0.1f, 0.4f, 0.6f, 1.0f);
	light3.SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	light3.SetPosition(0.0f, 50.0f, 0.0f);
	light3.SetDirection(0.0f, -1.0f, 0.0f);
	light3.SetExponent(5.0f);
	light3.SetCutoff(10.0f);

	SetAmbientMaterial(0.6f, 0.6f, 0.6f, 1.0f);
	SetDiffuseMaterial(0.4f, 0.4f, 0.4f, 1.0f);
	SetSpecularMaterial(0.1f, 0.1f, 0.1f, 1.0f);
}

void World::SetAmbientMaterial(float r, float g, float b, float a) {
	mAmbientMaterial[0] = r;
	mAmbientMaterial[1] = g;
	mAmbientMaterial[2] = b;
	mAmbientMaterial[3] = a;
}

void World::SetDiffuseMaterial(float r, float g, float b, float a) {
	mDiffuseMaterial[0] = r;
	mDiffuseMaterial[1] = g;
	mDiffuseMaterial[2] = b;
	mDiffuseMaterial[3] = a;
}

void World::SetSpecularMaterial(float r, float g, float b, float a) {
	mSpecularMaterial[0] = r;
	mSpecularMaterial[1] = g;
	mSpecularMaterial[2] = b;
	mSpecularMaterial[3] = a;
}


void World::render(glm::mat4 matrix, glm::vec3 cameraPos)
{
	// 渲染天空盒
	skyBox.render();

	glEnable(GL_CULL_FACE);

	// 渲染区块
	Shader s = ResourceManager::GetShader("shader_chunk");
	s.Use();
	s.SetInteger("tex", 0);
	s.SetMatrix4("matrix", matrix);

	Texture2DArray textureArray = ResourceManager::GetTextureArray("blocks");
	textureArray.Bind();

	for (auto &chunk : this->chunks) {
		chunk->render();
	}

	glUseProgram(0);

	if (picked) {
		drawWireCube(pickedBlock.x, pickedBlock.y, pickedBlock.z, matrix);
	}

	

	// 设置简单的光照
	//设置光照
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mAmbientMaterial);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mDiffuseMaterial);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mSpecularMaterial);
	glEnable(GL_COLOR_MATERIAL);
	//light.Enable();
	light1.Enable();
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

	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
}