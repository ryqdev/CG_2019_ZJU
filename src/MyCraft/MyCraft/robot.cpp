#include "robot.h"

#define PI 3.1415926535898
using namespace std;


Robot::Robot(int x, int y, int z,float s)
{
	speed = 0.1;
	random_t = 5;
	count = 0;
	vangle = 0;    // ËÙ¶ÈÆ«½Ç
	move = false;
	size = s;
	setLocation(x, y, z);
}

Robot::Robot(){}

void Robot::setLocation(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

void Robot::randomMove()
{
	if (this->count % this->random_t == 0)
	{
		this->random_t = rand() % 10 + 100;
		this->vangle += rand() % 100;
	}
	this->count++;
	this->x += this->speed * cos(this->vangle / 180 * PI);
	this->z -= this->speed * sin(this->vangle / 180 * PI);
}

GLint RobotRender::createList()
{
	GLint lid = glGenLists(1);
	glNewList(lid, GL_COMPILE);
	
	glPushMatrix();
	///////////////////////////Head////////////////////////////
	// head
	glColor3f(128.0 / 255, 42.0 / 255, 42.0 / 255);
	glPushMatrix();
	glTranslatef(0, 2.8,0);
	glScalef(0.7, 1, 1);
	glutSolidCube(1);
	glPopMatrix();

	// left eye
	glColor3f(0, 0, 0);
	glPushMatrix();
	glTranslatef(0.35,2.8, 0.5);
	glBegin(GL_QUADS);
	glVertex3f(0,0.1,-0.1); glVertex3f(0, 0.1, 0.1); glVertex3f(0, -0.1, 0.1); glVertex3f(0, -0.1, -0.1);
	glEnd();
	glPopMatrix();

	// right eye
	glColor3f(0, 0, 0);
	glPushMatrix();
	glTranslatef(0.35, 2.8, -0.5);
	glBegin(GL_QUADS);
	glVertex3f(0, 0.1, -0.1); glVertex3f(0, 0.1, 0.1); glVertex3f(0, -0.1, 0.1); glVertex3f(0, -0.1, -0.1);
	glEnd();
	glPopMatrix();

	// mouth
	glColor3f(0, 0, 0);
	glPushMatrix();
	glTranslatef(0.35, 2.4, 0.5);
	glBegin(GL_LINE);
	glVertex3f(0, 0.1, -0.2); glVertex3f(0, 0, -0.2); glVertex3f(0, 0, 0.2); glVertex3f(0, 0.1, 0.2);
	glEnd();
	glPopMatrix();


	///////////////////////////Body////////////////////////////
	glColor3f(150.0 / 255, 110.0 / 255, 110.0 / 255);
	glPushMatrix();
	glTranslatef(0, 1.3, 0);
	glScalef(0.7, 2, 0.8);
	glutSolidCube(1);
	glPopMatrix();

	/////////////////////////Foot//////////////////////////////
	glColor3f(128.0 / 255, 42.0 / 255, 42.0 / 255);
	glPushMatrix();
	glTranslatef(0, 0.15, 0);
	glScalef(0.7,0.3,0.9);
	glutSolidCube(1);
	glPopMatrix();


	glPopMatrix();
	glEndList();

	return lid;
}

void RobotRender::DrawRobot(Robot& robot) {
	
	glPushMatrix();
	glTranslatef(robot.x,robot.y,robot.z);
	glRotatef(robot.vangle, 0, 1, 0);
	glScalef(robot.size, robot.size, robot.size);
	
	glCallList(lid);

	glPopMatrix();
}

RobotRender::RobotRender() {
	for (int i = 0; i < N_ROBOT; i++) {
		int x = rand() % 16;
		int z = rand() % 16;
		robotList[i] = Robot(x,0,z);
	}
   lid = createList();
}