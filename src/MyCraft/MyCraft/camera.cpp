#include "camera.h"

// 跳跃功能
// const int initial_speed = 8;
bool isJump = false;
float speedY = 0;

void Camera::updateCameraVectors()
{
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
	front.y = sin(glm::radians(this->Pitch));
	front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
	this->Front = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	this->Up = glm::normalize(glm::cross(this->Right, this->Front));
}

GLfloat Camera::getZooom()
{
	return this->Zoom;
}

void Camera::setLookAt()
{
	gluLookAt(Position.x, Position.y, Position.z,
		(Position + Front).x, (Position + Front).y, (Position + Front).z,
		Up.x, Up.y, Up.z);
}

void Camera::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch)
{
	xoffset *= this->MouseSensitivity;
	yoffset *= this->MouseSensitivity;

	this->Yaw += xoffset;
	this->Pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (this->Pitch > 89.0f)
			this->Pitch = 89.0f;
		if (this->Pitch < -89.0f)
			this->Pitch = -89.0f;
	}

	// Update Front, Right and Up Vectors using the updated Eular angles
	this->updateCameraVectors();
}

void Camera::doMovement(GLboolean Keys[], World* world, GLfloat deltaTime)
{
	glm::vec3 tmp = this->Position;

	GLfloat velocity = this->MovementSpeed * deltaTime;
	if (Keys['w'])
		this->Position += this->Front * velocity;
	if (Keys['s'])
		this->Position -= this->Front * velocity;
	if (Keys['a'])
		this->Position -= this->Right * velocity;
	if (Keys['d'])
		this->Position += this->Right * velocity;
	if (Keys[' '] && !isJump && !flying) {
		isJump = true;
		speedY = 1.8;
	}

	if (!flying) 
	{
		// 自由落体
		Position.y = tmp.y;		// 保证键盘控制不能改变 y 
		speedY = speedY - 0.016;
	}

	// 下落速度最大值
	if (speedY < -10) {
		speedY = -10;
	}

	// 更新位置 y
	this->Position.y += speedY * deltaTime;

	// 碰撞检测
	if (this->doCollisions(world)) {
		speedY = 0;
		isJump = false;
	}
}

void Camera::doZoom(GLboolean Keys[], GLfloat deltaTime)
{
	if (Keys['k']) {
		this->Zoom = 35.0f;
	}
	else {
		this->Zoom = 65.0f;
	}
}

void Camera::ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
{
	glm::vec3 tmp = this->Position;
	GLfloat velocity = this->MovementSpeed * deltaTime;
	if (direction == FORWARD) {
		this->Position += this->Front * velocity;
	}
	if (direction == BACKWARD)
		this->Position -= this->Front * velocity;
	if (direction == LEFT)
		this->Position -= this->Right * velocity;
	if (direction == RIGHT)
		this->Position += this->Right * velocity;
	if (direction == JUMP && !isJump)
	{
		isJump = true;
		speedY = 2;
		// glutTimerFunc(20, updateCameraPosition, 1);
	}
	Position.y = tmp.y;		// 保证键盘控制不能改变 y 
}


bool Camera::doCollisions(World* world)
{
	bool result = false;

	float* x = &this->Position[0];
	float* y = &this->Position[1];
	float* z = &this->Position[2];

	int nx = roundf(*x);
	int ny = roundf(*y);
	int nz = roundf(*z);

	float px = *x - nx;
	float py = *y - ny;
	float pz = *z - nz;
	float pad = 0.25;

	for (int dy = 0; dy < 2; dy++) {
		if (px < -pad && world->getChunkManager().getBlock(nx - 1, ny - dy, nz) != AIR) {
			*x = nx - pad;
		}
		if (px > pad && world->getChunkManager().getBlock(nx + 1, ny - dy, nz) != AIR) {
			*x = nx + pad;
		}
		if (py < -pad && world->getChunkManager().getBlock(nx, ny - dy - 1, nz) != AIR) {
			*y = ny - pad;
			result = true;
		}
		if (py > pad && world->getChunkManager().getBlock(nx, ny - dy + 1, nz) != AIR) {
			*y = ny + pad;
			result = true;
		}
		if (pz < -pad && world->getChunkManager().getBlock(nx, ny - dy, nz - 1) != AIR) {
			*z = nz - pad;
		}
		if (pz > pad && world->getChunkManager().getBlock(nx, ny - dy, nz + 1) != AIR) {
			*z = nz + pad;
		}
	}
	return result;
}

void Camera::camera_mouse_callback(int xpos, int ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	// 获取窗口大小
	float wwidth = glutGet(GLUT_WINDOW_WIDTH);
	float wheight = glutGet(GLUT_WINDOW_HEIGHT);

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to left

	if (!showCurser)
	{
		lastX = wwidth / 2;
		lastY = wheight / 2;

		this->ProcessMouseMovement(xoffset, yoffset);

		// 获取了鼠标了位移量后，将鼠标移回屏幕中心!!!
		glutWarpPointer(wwidth / 2, wheight / 2);
	}
	else
	{
		lastX = xpos;
		lastY = ypos;

		this->ProcessMouseMovement(xoffset, yoffset);
	}

}

