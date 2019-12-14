#pragma once
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "camera.h"

/*
根据鼠标位置计算出在三维世界中的射线，主要参考
https://www.youtube.com/watch?v=DLKN0jExRIM
*/

struct IntersectPoint
{
	float x;
	float y;
	float z;
	bool isIntersect;
	float distance;
	glm::vec3 dir;

	IntersectPoint(float x, float y, float z, bool isIntersect, float distance, glm::vec3 dir) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->isIntersect = isIntersect;
		this->distance = distance;
		this->dir = dir;
	}

};

class MousePicker
{
private:
	glm::vec3 currentRay;

	glm::mat4 projectionMatrix;
	glm::mat4 viewMatrix;

	float mouseX, mouseY;
	float width, height;


	Camera* camera;

	glm::vec2 getNormalizedDeviceCoords();

	glm::vec4 toEyeCoords(glm::vec4 clipCoords);

	glm::vec3 toWorldCoords(glm::vec4 eyeCoords);

	glm::vec3 calculateMouseRay();

public:
	MousePicker(Camera* camera, glm::mat4 projection);

	glm::vec3 getCurrentRay();

	// 需要在鼠标移动的时候调用该函数更新
	void update(float mouseX, float mouseY, float width, float height);

	// 测试鼠标射线
	void render_ray();

	// 判断当前射线与立方体那个面相交
	// 输入射线方向，相机位置，单位立方体中心坐标
	// 返回
	glm::vec3 Intersect(glm::vec3 ray, glm::vec3 camera, int x, int y, int z);
};