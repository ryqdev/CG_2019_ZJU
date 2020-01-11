#include "robot.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>


#define PI 3.1415926535898
using namespace std;


Robot::Robot(int x, int y, int z, float s)
{
	speed = 0.1;
	random_t = 5;
	count = 0;
	vangle = 0;    // 速度偏角
	move = false;
	size = s;
	setLocation(x, y, z);
}

Robot::Robot() {}

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
//GLfloat cubenormals[36][3] = {
//
//		0,0,-1,
//		0,0,-1,
//		0,0,-1,
//		0,0,-1,
//		0,0,-1,
//		0,0,-1,
//
//		0,0,1,
//		0,0,1,
//		0,0,1,
//		0,0,1,
//		0,0,1,
//		0,0,1,
//
//		-1,0,0,
//		-1,0,0,
//		-1,0,0,
//		-1,0,0,
//		-1,0,0,
//		-1,0,0,
//
//		1,0,0,
//		1,0,0,
//		1,0,0,
//		1,0,0,
//		1,0,0,
//		1,0,0,
//
//		0,-1,0,
//		0,-1,0,
//		0,-1,0,
//		0,-1,0,
//		0,-1,0,
//		0,-1,0,
//
//		0,1,0,
//		0,1,0,
//		0,1,0,
//		0,1,0,
//		0,1,0,
//		0,1,0,
//};
//
//GLfloat cubeverts[36][3] = {
//		-1.0f,     -1.0f,     -1.0f,
//		1.0f,     -1.0f,     -1.0f,
//		1.0f,     1.0f,     -1.0f,
//		1.0f,     1.0f,     -1.0f,
//		-1.0f,     1.0f,     -1.0f,
//		-1.0f,     -1.0f,     -1.0f,
//
//		-1.0f,     -1.0f,     1.0f,
//		1.0f,     -1.0f,     1.0f,
//		1.0f,     1.0f,     1.0f,
//		1.0f,     1.0f,     1.0f,
//		-1.0f,     1.0f,     1.0f,
//		-1.0f,     -1.0f,     1.0f,
//
//		-1.0f,     1.0f,     1.0f,
//		-1.0f,     1.0f,     -1.0f,
//		-1.0f,     -1.0f,     -1.0f,
//		-1.0f,     -1.0f,     -1.0f,
//		-1.0f,     -1.0f,     1.0f,
//		-1.0f,     1.0f,     1.0f,
//
//		1.0f,     1.0f,     1.0f,
//		1.0f,     1.0f,     -1.0f,
//		1.0f,     -1.0f,     -1.0f,
//		1.0f,     -1.0f,     -1.0f,
//		1.0f,     -1.0f,     1.0f,
//		1.0f,     1.0f,     1.0f,
//
//		-1.0f,     -1.0f,     -1.0f,
//		1.0f,     -1.0f,     -1.0f,
//		1.0f,     -1.0f,     1.0f,
//		1.0f,     -1.0f,     1.0f,
//		-1.0f,     -1.0f,     1.0f,
//		-1.0f,     -1.0f,     -1.0f,
//
//		-1.0f,     1.0f,     -1.0f,
//		1.0f,     1.0f,     -1.0f,
//		1.0f,     1.0f,     1.0f,
//		1.0f,     1.0f,     1.0f,
//		-1.0f,     1.0f,     1.0f,
//		-1.0f,     1.0f,     -1.0f,
//};
//
//GLfloat cubeTex[36][3] = {
//
//};
void RobotRender::initBuffer()
{
	sphere = new Sphere();
	//sphere->printSelf();
	// bind vbo for flat sphere (left)
	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);
	//std::cout << vao << std::endl;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	int stride = sphere->getInterleavedStride();
	glBufferData(GL_ARRAY_BUFFER, sphere->getInterleavedVertexSize(), sphere->getInterleavedVertices(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphere->getIndexSize(), sphere->getIndices(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	//
}
//void RobotRender::initBuffer()
//{
//	vector<glm::vec3> vertlist;
//	vector<glm::vec3> colorlist;
//	vector<glm::vec3> normallist;
//
//
//	glPushMatrix();
//	///////////////////////////Head////////////////////////////
//	// head
//	//glColor3f(128.0 / 255, 42.0 / 255, 42.0 / 255);
//	//glPushMatrix();
//
//	{
//
//		/*	glm::vec4(0, 2.8, 0, 1);
//			glm::vec4(0.7, 1, 1, 1);*/
//			//glTranslatef(0, 2.8, 0);
//			//glScalef(0.7, 1, 1);
//		glm::mat4 m(1.0);
//		m = glm::translate(m, glm::vec3(0, 2.8, 0));
//		m = glm::scale(m, glm::vec3(0.7, 1., 1.));
//
//
//		glm::vec3 verts[36];
//
//		for (int i = 0; i < 36; i++)
//		{
//			glm::vec4 temp4 = m * glm::vec4(cubeverts[i][0], cubeverts[i][1], cubeverts[i][2], 1.0);
//			glm::vec3 temp = glm::vec3(temp4);
//			//	std::cout << cubeverts[i][0] << cubeverts[i][1] << cubeverts[i][2] << 1.0 << std::endl;
//				///	std::cout << glm::to_string(temp4) << std::endl;
//				//	std::cout << glm::to_string(temp) << std::endl;
//			vertlist.push_back(temp);
//		}
//
//		for (int i = 0; i < 36; i++)
//		{
//			glm::vec3 temp(128.0 / 255, 42.0 / 255, 42.0 / 255);
//			colorlist.push_back(temp);
//		}
//
//		for (int i = 0; i < 36; i++)
//		{
//			glm::vec3 temp = glm::vec3(cubenormals[i][0], cubenormals[i][1], cubenormals[i][2]);
//			normallist.push_back(temp);
//		}
//	}
//	//	glutSolidCube(1);
//	//glPopMatrix();
//
//	GLfloat eyevert[6][3] = {
//		0, 0.1, -0.1,
//		0, 0.1, 0.1,
//		0, -0.1, 0.1,
//		0, 0.1, -0.1,
//		0, -0.1, 0.1,
//		0, -0.1, -0.1
//	};
//	// left eye
//	{
//		glm::mat4 m = glm::mat4(1.0);
//		m = glm::translate(m, glm::vec3(0.35, 2.8, 0.5));
//
//
//		for (int i = 0; i < 6; i++)
//		{
//			glm::vec4 temp4 = m * glm::vec4(eyevert[i][0], eyevert[i][1], eyevert[i][2], 1.0);
//			glm::vec3 temp = glm::vec4(temp4);
//			vertlist.push_back(temp);
//		}
//		for (int i = 0; i < 6; i++)
//		{
//			glm::vec3 temp(0, 0, 0);
//			colorlist.push_back(temp);
//		}
//
//		for (int i = 0; i < 6; i++)
//		{
//			glm::vec3 temp = glm::vec3(1, 0, 0);
//			normallist.push_back(temp);
//		}
//	}
//	/*glColor3f(0, 0, 0);
//	glPushMatrix();
//	glTranslatef(0.35, 2.8, 0.5);
//	glBegin(GL_QUADS);
//	glVertex3f(0, 0.1, -0.1); glVertex3f(0, 0.1, 0.1); glVertex3f(0, -0.1, 0.1);
//	glVertex3f(0, 0.1, -0.1); glVertex3f(0, -0.1, 0.1); glVertex3f(0, -0.1, -0.1);
//
//	glEnd();
//	glPopMatrix();*/
//
//
//	// right eye
//
//	{
//		glm::mat4 m = glm::mat4(1.0);
//		m = glm::translate(m, glm::vec3(0.35, 2.8, -0.5));
//
//
//		for (int i = 0; i < 6; i++)
//		{
//			glm::vec4 temp4 = m * glm::vec4(eyevert[i][0], eyevert[i][1], eyevert[i][2], 1.0);
//			glm::vec3 temp = glm::vec3(temp4);
//			vertlist.push_back(temp);
//		}
//		for (int i = 0; i < 6; i++)
//		{
//			glm::vec3 temp(0, 0, 0);
//			colorlist.push_back(temp);
//		}
//
//		for (int i = 0; i < 6; i++)
//		{
//			glm::vec3 temp = glm::vec3(1, 0, 0);
//			normallist.push_back(temp);
//		}
//	}
//	//glColor3f(0, 0, 0);
//	//glPushMatrix();
//	//glTranslatef(0.35, 2.8, -0.5);
//	//glBegin(GL_QUADS);
//	//glVertex3f(0, 0.1, -0.1); glVertex3f(0, 0.1, 0.1); glVertex3f(0, -0.1, 0.1); glVertex3f(0, -0.1, -0.1);
//	//glEnd();
//	//glPopMatrix();
//
//	GLfloat mouseverts[6][3] =
//	{
//		0, 0.1, -0.2,
//		0, 0, -0.2,
//		0, 0, 0.2,
//		0, 0.1, -0.2,
//		0, 0, 0.2,
//		0, 0.1, 0.2,
//	};
//
//	// mouth
//	{
//		glm::mat4 m = glm::mat4(1.0);
//		m = glm::translate(m, glm::vec3(0.35, 2.4, 0.5));
//
//
//		for (int i = 0; i < 6; i++)
//		{
//			glm::vec4 temp4 = m * glm::vec4(mouseverts[i][0], mouseverts[i][1], mouseverts[i][2], 1.0);
//			glm::vec3 temp = glm::vec3(temp4);
//			vertlist.push_back(temp);
//		}
//		for (int i = 0; i < 6; i++)
//		{
//			glm::vec3 temp(0, 0, 0);
//			colorlist.push_back(temp);
//		}
//
//		for (int i = 0; i < 6; i++)
//		{
//			glm::vec3 temp = glm::vec3(1, 0, 0);
//			normallist.push_back(temp);
//		}
//	}
//	//glColor3f(0, 0, 0);
//	//glPushMatrix();
//	//glTranslatef(0.35, 2.4, 0.5);
//	//glBegin(GL_LINE);
//	//glVertex3f(0, 0.1, -0.2); glVertex3f(0, 0, -0.2); glVertex3f(0, 0, 0.2); glVertex3f(0, 0.1, 0.2);
//	//glEnd();
//	//glPopMatrix();
//
//
//	///////////////////////////Body////////////////////////////
//	//Body
//	{
//		glm::mat4 m = glm::mat4(1.0);
//		m = glm::translate(m, glm::vec3(0, 1.3, 0));
//		m = glm::scale(m, glm::vec3(0.7, 2, 0.8));
//
//
//		glm::vec3 verts[36];
//		for (int i = 0; i < 36; i++)
//		{
//			glm::vec4 temp4 = m * glm::vec4(cubeverts[i][0], cubeverts[i][1], cubeverts[i][2], 1.0);
//			glm::vec3 temp = glm::vec3(temp4);
//
//			vertlist.push_back(temp);
//		}
//
//		for (int i = 0; i < 36; i++)
//		{
//			glm::vec3 temp(150.0 / 255, 110.0 / 255, 110.0 / 255);
//			colorlist.push_back(temp);
//		}
//
//		for (int i = 0; i < 36; i++)
//		{
//			glm::vec3 temp = glm::vec3(cubenormals[i][0], cubenormals[i][1], cubenormals[i][2]);
//			normallist.push_back(temp);
//		}
//	}
//
//	/*glColor3f(150.0 / 255, 110.0 / 255, 110.0 / 255);
//	glPushMatrix();
//	glTranslatef(0, 1.3, 0);
//	glScalef(0.7, 2, 0.8);
//	glutSolidCube(1);
//	glPopMatrix();*/
//
//	/////////////////////////Foot//////////////////////////////
//	//foot
//	{
//		glm::mat4 m = glm::mat4(1.0);
//		m = glm::translate(m, glm::vec3(0, 0.15, 0));
//		m = glm::scale(m, glm::vec3(0.7, 0.3, 0.9));
//
//		glm::vec3 verts[36];
//		for (int i = 0; i < 36; i++)
//		{
//			glm::vec4 temp4 = m * glm::vec4(cubeverts[i][0], cubeverts[i][1], cubeverts[i][2], 1.0);
//			glm::vec3 temp = glm::vec3(temp4);
//			vertlist.push_back(temp);
//		}
//
//		for (int i = 0; i < 36; i++)
//		{
//			glm::vec3 temp(128.0 / 255, 42.0 / 255, 42.0 / 255);
//			colorlist.push_back(temp);
//		}
//
//		for (int i = 0; i < 36; i++)
//		{
//			glm::vec3 temp = glm::vec3(cubenormals[i][0], cubenormals[i][1], cubenormals[i][2]);
//			normallist.push_back(temp);
//		}
//	}
//
//	//glColor3f(128.0 / 255, 42.0 / 255, 42.0 / 255);
//	//glPushMatrix();
//	//glTranslatef(0, 0.15, 0);
//	//glScalef(0.7, 0.3, 0.9);
//	//glutSolidCube(1);
//	//glPopMatrix();
//
//
//	//glPopMatrix();
//	//glEndList();
//	std::vector<float>  data;
//	for (int i = 0; i < vertlist.size(); i++)
//	{
//		data.push_back(vertlist[i].x);
//		data.push_back(vertlist[i].y);
//		data.push_back(vertlist[i].z);
//
//		data.push_back(normallist[i].x);
//		data.push_back(normallist[i].y);
//		data.push_back(normallist[i].z);
//
//		data.push_back(colorlist[i].x);
//		data.push_back(colorlist[i].y);
//		data.push_back(colorlist[i].z);
//	}
//
//
//	// vao与vbo
//	glGenVertexArrays(1, &vao);
//	glBindVertexArray(vao);
//	GLuint vbo;
//	glGenBuffers(1, &vbo);
//	glBindBuffer(GL_ARRAY_BUFFER, vbo);
//	glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)0);
//	glEnableVertexAttribArray(1);
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)(3 * sizeof(float)));
//	glEnableVertexAttribArray(2);
//	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 9, (void*)(6 * sizeof(float)));
//	glBindVertexArray(0);
//
//	count = vertlist.size();
//	vertlist.clear();
//	normallist.clear();
//	colorlist.clear();
//}


GLint RobotRender::createList()
{
	GLint lid = glGenLists(1);
	glNewList(lid, GL_COMPILE);

	glPushMatrix();
	///////////////////////////Head////////////////////////////
	// head
	glColor3f(128.0 / 255, 42.0 / 255, 42.0 / 255);
	glPushMatrix();
	glTranslatef(0, 2.8, 0);
	glScalef(0.7, 1, 1);
	glutSolidCube(1);
	glPopMatrix();

	// left eye
	glColor3f(0, 0, 0);
	glPushMatrix();
	glTranslatef(0.35, 2.8, 0.5);
	glBegin(GL_QUADS);
	glVertex3f(0, 0.1, -0.1); glVertex3f(0, 0.1, 0.1); glVertex3f(0, -0.1, 0.1); glVertex3f(0, -0.1, -0.1);
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
	glScalef(0.7, 0.3, 0.9);
	glutSolidCube(1);
	glPopMatrix();


	glPopMatrix();
	glEndList();

	return lid;
}

void RobotRender::DrawRobot(Shader s, Robot& robot) {

	s.Use();
	glm::mat4 model(1.0);
	model = glm::translate(model, glm::vec3(robot.x, robot.y, robot.z));
	model = glm::rotate(model, robot.vangle, glm::vec3(0, 1, 0));
	model = glm::scale(model, glm::vec3(robot.size, robot.size, robot.size));
	//std::cout << robot.size << std::endl;
	//	std::cout << glm::to_string(model) << std::endl;
	s.SetMatrix4("model", model);
	//	glCallList(lid);

	sphere->draw();
	//	glPopMatrix();
}

void RobotRender::DrawBall()
{

	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glDrawElements(GL_TRIANGLES,  // primitive type
		sphere->getIndexCount(), // # of indices
		GL_UNSIGNED_INT, // data type
		(void*)0); // ptr to indices
	glBindVertexArray(0);

}

float rr = 0;
void RobotRender::DrawRobot(Shader s, Robot& robot, Texture2D& tex0, Texture2D& tex1)
{
	s.Use();
	glm::mat4 model(1.0);
	model = glm::translate(model, glm::vec3(robot.x, robot.y + 1.0, robot.z));
	model = glm::rotate(model, robot.vangle, glm::vec3(0, 1, 0));
	model = glm::rotate(model, rr, glm::vec3(1, 0, 0));
	model = glm::scale(model, glm::vec3(robot.size, robot.size, robot.size));
	//std::cout << robot.size << std::endl;
	//	std::cout << glm::to_string(model) << std::endl;
	s.SetMatrix4("model", model);
	tex0.Bind();
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glDrawElements(GL_TRIANGLES,  // primitive type
		sphere->getIndexCount(), // # of indices
		GL_UNSIGNED_INT, // data type
		(void*)0); // ptr to indices
	glBindVertexArray(0);
	model = glm::mat4(1.0);
	//model = glm::translate(model, glm::vec3(0, robot.y, 0));
	//model = glm::scale(model, glm::vec3(0.5,0.5,0.5));
	model = glm::translate(model, glm::vec3(robot.x, robot.y + 2.50, robot.z));
	model = glm::rotate(model, robot.vangle, glm::vec3(0, 1, 0));
	model = glm::rotate(model, -rr, glm::vec3(1, 0, 0));
	model = glm::scale(model, glm::vec3(robot.size * 0.5, robot.size * 0.5, robot.size * 0.5));
	tex1.Bind();
	s.SetMatrix4("model", model);
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glDrawElements(GL_TRIANGLES,            // primitive type
		sphere->getIndexCount(), // # of indices
		GL_UNSIGNED_INT,         // data type
		(void*)0);               // ptr to indices
	glBindVertexArray(0);
	//	glCallList(lid);

	rr += 0.10f;
	rr = rr > 360 ? rr - 360 : rr;
}

RobotRender::RobotRender() {
	for (int i = 0; i < N_ROBOT; i++) {
		int x = rand() % 100;
		int z = rand() % 100;
		robotList[i] = Robot(x, 0, z);
	}

	initBuffer();//初始化机器人的vbo
	//lid = createList();
}
