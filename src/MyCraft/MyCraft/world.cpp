#include "world.h"
#include "resource_manager.h"
#include "light.h"

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

World::~World()
{
	// 释放渲染器
	if (cubeRender != nullptr)
		delete cubeRender;
	//释放树木渲染器
	if ( treeRender!= nullptr)
		delete treeRender;
}

ChunkManager& World::getChunkManager()
{
	return chunkManager;
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

void World::init()
{
	// 初始化立方体渲染器
	cubeRender = new CubeRender();
	
	// 初始化树木渲染器
	treeRender = new TreeRender();
	
	// 初始化机器人
	robotRender=new RobotRender();

	// 初始化天空盒
	skyBox.init(ResourceManager::GetShader("shader_skybox"));

	// 初始化区块
	chunkManager.init(ResourceManager::GetShader("shader_chunk"));
	for (int i = -2; i <= 2; i++) {
		for (int j = -2; j <= 2; j++) {
			chunkManager.getChunk(i, j);
		}
	}
	
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

void World::render(Camera& camera)
{
	// 渲染天空盒
	skyBox.render();

	chunkManager.render(camera.Position, camera.getProjViewMatrix());

	glEnable(GL_CULL_FACE);


	if (picked) {
		cubeRender->drawWireCube(pickedBlock.x, pickedBlock.y, pickedBlock.z, camera.getProjViewMatrix());
	}

	glUseProgram(0);
	
	//渲染树木 最多N_TREE数量树木
	Texture2D trunk= ResourceManager::GetTexture("trunk");
	Texture2D leaves = ResourceManager::GetTexture("leaves");
	for (int i = 0; i < N_TREE; i++) {
		Tree t(treeRender->treelist[i][0], chunkManager.highest(treeRender->treelist[i][0], treeRender->treelist[i][1]), treeRender->treelist[i][1]);
		treeRender->DrawTree(t, trunk, leaves);
	}
	
	//渲染robot
	for (int i = 0; i < N_ROBOT; i++) {
		robotRender->robotList[i].randomMove();
		float x = robotRender->robotList[i].x;
		float z = robotRender->robotList[i].z;
		robotRender->robotList[i].setLocation(x,chunkManager.highest(x,z),z);
		robotRender->DrawRobot(robotRender->robotList[i]);
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