#include "mousePicker.h"

MousePicker::MousePicker(Camera* camera, glm::mat4 projection)
{
	this->currentRay = glm::vec3(0);
	this->camera = camera;
	this->projectionMatrix = projection;
	this->viewMatrix = camera->GetViewMatrix();
	this->mouseX = 0;
	this->mouseY = 0;
	this->width = 0;
	this->height = 0;
}


glm::vec3 MousePicker::getCurrentRay()
{
	return this->currentRay;
}

void MousePicker::update(float x, float y, float width, float height)
{
	this->mouseX = x;
	this->mouseY = y;
	this->width = width;
	this->height = height;
	this->viewMatrix = camera->GetViewMatrix();
	this->currentRay = calculateMouseRay();
}

glm::vec3 MousePicker::calculateMouseRay()
{
	glm::vec2 normalizeCoords = this->getNormalizedDeviceCoords();
	glm::vec4 clipCoords = glm::vec4(normalizeCoords.x, normalizeCoords.y, -1.0f, 1.0f);
	glm::vec4 eyeCoords = toEyeCoords(clipCoords);
	glm::vec3 worldRay = toWorldCoords(eyeCoords);
	return  worldRay;
}

glm::vec4 MousePicker::toEyeCoords(glm::vec4 clipCoords)
{
	glm::mat4 invertedProjection = glm::inverse(this->projectionMatrix);
	glm::vec4 eyeCoords = invertedProjection * clipCoords;
	return glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);
}

glm::vec3 MousePicker::toWorldCoords(glm::vec4 eyeCoords)
{
	glm::mat4 invertedView = glm::inverse(this->viewMatrix);
	glm::vec4 rayWorld = invertedView * eyeCoords;
	glm::vec3 mouseRay = glm::vec3(rayWorld.x, rayWorld.y, rayWorld.z);
	glm::normalize(mouseRay);
	return mouseRay;
}

glm::vec2 MousePicker::getNormalizedDeviceCoords()
{
	float x = (2 * this->mouseX) / this->width - 1;

	// float y = (2 * this->mouseY) / this->height - 1;
	// 不同的设备的不同坐标系统要注意
	float y = 1 - (2 * this->mouseY) / this->height;

	return glm::vec2(x, y);
}

void MousePicker::render_ray()
{
	// glEnable(GL_LIGHTING);
	// GLfloat gray[] = { 0.4, 0.4, 0.4, 1.0 };
	// GLfloat light_pos[] = { camera->Position.x, camera->Position.y, camera->Position.z, 1 };

	glm::vec3 ray = getCurrentRay();
	
	/*
	cout << ray.x << " " << ray.y << " " << ray.z << endl;
	
	float startX = camera->Position.x + ray.x * 10;
	float startY = camera->Position.y + ray.y * 10;
	float startZ = camera->Position.z + ray.z * 10;
	float endX = camera->Position.x + ray.x * 100;
	float endY = camera->Position.y + ray.y * 100;
	float endZ = camera->Position.z + ray.z * 100;

	glPushMatrix();
	glLoadIdentity();
	glBegin(GL_LINES);
		glVertex3f(startX, startY, startZ);
		glVertex3f(endX, endY, endZ);
	glEnd();
	glPopMatrix();
	*/


}