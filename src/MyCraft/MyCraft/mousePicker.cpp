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

glm::vec3 MousePicker::Intersect(glm::vec3 ray, glm::vec3 camera, int  x, int y, int z)
{
	float u_up = (y + 0.5 - camera.y) / ray.y;
	float x_up = camera.x + u_up * ray.x;
	float z_up = camera.z + u_up * ray.z;
	float dis_up = glm::distance(camera, glm::vec3(x_up, y + 0.5, z_up));
	bool ins_up = (x_up > x- 0.5 && x_up < x + 0.5 && z_up > z - 0.5 && z_up < z + 0.5);
	IntersectPoint up(x_up, y + 0.5, z_up, ins_up, dis_up, glm::vec3(0, 1, 0));


	float u_down = (y + 0.5 - camera.y) / ray.y;
	float x_down = camera.x + u_down * ray.x;
	float z_down = camera.z + u_down * ray.z;
	float dis_down = glm::distance(camera, glm::vec3(x_down, y - 0.5, z_down));
	bool ins_down = (x_down > x - 0.5 && x_down < x + 0.5 && z_down > z - 0.5 && z_down < z + 0.5);
	IntersectPoint down(x_down, y - 0.5, z_down, ins_down, dis_down, glm::vec3(0, -1, 0));


	float u_left = (x - 0.5 - camera.x) / ray.x;
	float y_left = camera.y + u_left * ray.y;
	float z_left = camera.z + u_left * ray.z;
	float dis_left = glm::distance(camera, glm::vec3(x - 0.5, y_left, z_left));
	bool ins_left = (y_left > y - 0.5 && y_left < y + 0.5 && z_left < z + 0.5 && z_left > z - 0.5);
	IntersectPoint left(x - 0.5, y_left, z_left, ins_left, dis_left, glm::vec3(-1, 0, 0));


	float u_right = (x + 0.5 - camera.x) / ray.x;
	float y_right = camera.y + u_right * ray.y;
	float z_right = camera.z + u_right * ray.z;
	float dis_right = glm::distance(camera, glm::vec3(x + 0.5, y_right, z_right));
	bool ins_right = (y_right > y - 0.5 && y_right < y + 0.5 && z_right < z + 0.5 && z_right > z - 0.5);
	IntersectPoint right(x - 0.5, y_right, z_right, ins_right, dis_right, glm::vec3(1, 0, 0));

	float u_front = (z + 0.5 - camera.z) / ray.z;
	float x_front = camera.x + u_front * ray.x;
	float y_front = camera.y + u_front * ray.y;
	float dis_front = glm::distance(camera, glm::vec3(x_front, y_front, z + 0.5));
	bool ins_front = (x_front > x - 0.5 && x_front < x + 0.5 && y_front < y + 0.5 && y_front > y - 0.5);
	IntersectPoint front(x_front, y_front, z + 0.5, ins_front, dis_front, glm::vec3(0, 0, 1));

	float u_back = (z - 0.5 - camera.z) / ray.z;
	float x_back = camera.x + u_back * ray.x;
	float y_back = camera.y + u_back * ray.y;
	float dis_back = glm::distance(camera, glm::vec3(x_back, y_back, z - 0.5));
	bool ins_back = (x_back > x - 0.5 && x_back < x + 0.5 && y_back < y + 0.5 && y_back > y - 0.5);
	IntersectPoint back(x_back, y_back, z - 0.5, ins_back, dis_back, glm::vec3(0, 0, -1));

	vector<IntersectPoint> points;
	points.push_back(up);
	points.push_back(down);
	points.push_back(left);
	points.push_back(right);
	points.push_back(front);
	points.push_back(back);

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5 - i; j++) {
			if (points[j].distance > points[j + 1].distance) {
				IntersectPoint tmp = points[j + 1];
				points[j + 1] = points[j];
				points[j] = tmp;
			}
		}
	}

	for (int i = 0; i < 6; i++) {
		if (points[i].isIntersect) {
			return points[i].dir;
		}
	}
	return glm::vec3(0, 0, 0);

}