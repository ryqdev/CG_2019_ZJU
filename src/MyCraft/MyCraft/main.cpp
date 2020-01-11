// 让 fopen 不会报错,或者向 C/C++, 预处理器，预处理器定义中 添加 _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#define GLEW_STATIC
#include "GL/glew.h"

#include "game.h"
#include "screenshot.h"

//time
time_t rawtime;
struct tm* ptminfo;

// The Width of the screen
const GLuint SCREEN_WIDTH = 1024;
// The height of the screen
const GLuint SCREEN_HEIGHT = 640;

Game MineCraft(SCREEN_WIDTH, SCREEN_HEIGHT);

// the screen size
int screenWidth, screenHeight;

void reshape(int w, int h)
{
	screenWidth = w;
	screenHeight = h;
	MineCraft.Reshape(w, h);
	glViewport(0, 0, w, h);
}

void idle()
{
	// 处理用户的输入
	MineCraft.ProcessInput(0.05);
	// 更新游戏
	MineCraft.Update(0.01);

	// 重绘画面
	glutPostRedisplay();
}

void timer(int extra)
{
	// 处理用户的输入
	MineCraft.ProcessInput(0.01);
	// 更新游戏
	MineCraft.Update(0.01);

	// 重绘画面
	glutPostRedisplay();
	// 每 5ms 调用一次该函数， 主要用于控制帧率fps
	glutTimerFunc(5, timer, 0);
}

void getFPS()
{
	static int frame = 0, time, timebase = 0;
	static char buffer[256];

	frame++;
	time = glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000) {
		sprintf_s(buffer, "FPS:%4.2f ", frame * 1000.0 / (time - timebase));
		timebase = time;
		frame = 0;
	}
	// cout << buffer << endl;
	char* c;
	glDisable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);  // 选择投影矩阵
	glPushMatrix();               // 保存原矩阵
	glLoadIdentity();             // 装入单位矩阵
	glOrtho(0, 480, 0, 480, -1, 1);    // 位置正投影
	glMatrixMode(GL_MODELVIEW);   // 选择Modelview矩阵
	glPushMatrix();               // 保存原矩阵
	glLoadIdentity();             // 装入单位矩阵
	glRasterPos2f(10, 10);
	for (c = buffer; *c != '\0'; c++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
	}
	glMatrixMode(GL_PROJECTION);  // 选择投影矩阵
	glPopMatrix();                // 重置为原保存矩阵
	glMatrixMode(GL_MODELVIEW);   // 选择Modelview矩阵
	glPopMatrix();                // 重置为原保存矩阵
	glEnable(GL_DEPTH_TEST);
}

void display(void)
{
	// 渲染游戏画面
	MineCraft.Render();
	// 显示 FPS 信息
	getFPS();
	// 交换缓冲区
	glutSwapBuffers();
}

void keyDown(unsigned char key, int x, int y)
{
	if (key == 27)	// ESC
	{
		exit(0);
	}
	if (key == 'f')
	{
		MineCraft.camera->flying = !MineCraft.camera->flying;
	}
	if (key == 'q')	// Q 键切换要绘制的方块
	{
		MineCraft.nextBlcokType();
	}
	// 记录相应按键按下的状态，其中 K 键 Zoom，WASD移动相机
	if (key >= 0 && key < 1024)
	{
		MineCraft.Keys[key] = GL_TRUE;
	}

	if (key == 'p' || key == 'P')
	{
		time(&rawtime);
		ptminfo = localtime(&rawtime);
		Screenshot::save_img(ptminfo);
	}

	if (key == 'u' || key == 'U') {
		MineCraft.Init();
		MineCraft.file.Init();
	}
	if (key == 'i' || key == 'I')
	{
		MineCraft.Init();
		MineCraft.file.ReadFile();
		for (int i = 0; i < MineCraft.file.vx.size(); i++) {
			int ix = MineCraft.file.vx[i], iy = MineCraft.file.vy[i], iz = MineCraft.file.vz[i];
			//if (MineCraft.file.vtype[i] == -1) {
			//	MineCraft.world->remove_block(ix, iy, iz);
			//}
			//else {
			//	MineCraft.world->put_block(ix, iy, iz, BlockType(i));
			//}
			switch (MineCraft.file.vtype[i]) {
			case -1:MineCraft.world->remove_block(ix, iy, iz); break;
			case 0:MineCraft.world->put_block(ix, iy, iz, GRASS); break;
			case 1:MineCraft.world->put_block(ix, iy, iz, ICE); break;
			case 2:MineCraft.world->put_block(ix, iy, iz, GOLD); break;
			}
		}
	}
}

void keyUp(unsigned char key, int x, int y)
{
	if (key >= 0 && key < 1024)
	{
		MineCraft.Keys[key] = GL_FALSE;
	}
}

void init()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	// setting the clear color, seems like background
	glShadeModel(GL_SMOOTH);	// 着色器模式
	glEnable(GL_TEXTURE_2D);	// 打开纹理映射

								// 初始化游戏
	MineCraft.Init();
	//创建文件
	MineCraft.file.Init();
	glutTimerFunc(5, timer, 0);
	//timer(0);
}

void mouse_move_callback(int xpos, int ypos)
{
	MineCraft.MouseMoveCallback(xpos, ypos);
}

void  mouse_click_callback(int button, int state, int x, int y)
{
	MineCraft.MouseClickCallback(button, state, x, y);
}

int main(int argc, char* argv[])
{


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	glutInitWindowPosition(50, 50);
	glutCreateWindow("Minecraft");
	glutSetCursor(GLUT_CURSOR_NONE);

	glewExperimental = GL_TRUE;
	glewInit();
	glGetError(); // Call it once to catch glewInit() bug, all other errors are now from our application.

	init();

	// 当窗口内容绘制，窗口大小改变，窗口重绘时，会调用display函数
	glutDisplayFunc(display);

	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyDown);
	glutKeyboardUpFunc(keyUp);
	glutPassiveMotionFunc(mouse_move_callback);
	glutMouseFunc(mouse_click_callback);
	//glutIdleFunc(idle);
	glutMainLoop();
	return 0;
}
