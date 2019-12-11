#pragma once
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "camera.h"

/*
根据鼠标位置计算出在三维世界中的射线，主要参考
https://www.youtube.com/watch?v=DLKN0jExRIM
*/

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
};