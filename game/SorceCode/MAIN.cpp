#include "../glut/glut.h"
#define _USE_MATH_DEFINES
#include <list>
#include <vector>
#include "../Header/DEFINE.h"
#include "../Header/Player.h"
using namespace std;



GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
void TimerFunction(int value);
void Keyboard(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
void Drag(int x, int y);
void MousePos(int x, int y);


Player PC(100, WindowHei / 2, 0, 0, GREEN);
list<Projectile> p;

bool RDragCheck, LDragCheck;
int GState;

void main(int argc, char *argv[]) {

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WindowWid, WindowHei);
	glutCreateWindow("Proj : MSG(Added Replay)");
	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMotionFunc(Drag);
	glutPassiveMotionFunc(MousePos);
	glutTimerFunc(GameSpd, TimerFunction, 1);

	glutMainLoop();
}

GLvoid drawScene(GLvoid) {
	glClearColor(0.0, 0.0, 0.0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	switch (GState)
	{
	case TITLE:
	{
		char title[2][21]{ { "VECTOR RUSH" },{ "Game Start : key [1]" }};
		glColor3ub(0, 255, 0);
		glRasterPos2f(WindowWid / 2 - 100, WindowHei / 2 - 50);
		for (char *c = title[0]; *c != '\0'; ++c)
		{
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
		}
		glRasterPos2f(WindowWid / 2 - 100, WindowHei / 2 + 50);
		for (char *c = title[1]; *c != '\0'; ++c)
		{
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
		}
	}
	break;
	case PLAY:
		PC.draw();
		for (auto& d : p) {
			d.draw();
		}
		break;
	}
	glutSwapBuffers();
}

GLvoid Reshape(int w, int h) {
	glViewport(0, 0, w, h);
	glOrtho(0.0, WindowWid, WindowHei, 0.0, -100.0, 100.0);
}

void TimerFunction(int value) {
	switch (GState)
	{
	case PLAY:
		PC.update(0.01, LDragCheck, RDragCheck);

		if (PC.getStatus() == DEAD) {
			GState = END;
		}
	break;
	}
	glutPostRedisplay();
	glutTimerFunc(GameSpd, TimerFunction, 1);
}

void Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case '1':
		switch (GState) {
		case TITLE:
			PC.init(100, WindowHei / 2);
			RDragCheck = false;
			LDragCheck = false;
			GState = PLAY;
			break;
		}
		break;
	case '2':
		break;
	case '`':
		exit(0);
		break;
	}
}

void Mouse(int button, int state, int x, int y) {
	if (GState == PLAY) {
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
			RDragCheck = true;
		}
		else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
			RDragCheck = false;
		}
		if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
			LDragCheck = true;
		}
		else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
			LDragCheck = false;
		}
	}
}

void MousePos(int x, int y) {
	if (GState == PLAY) {
		PC.setDirX(x);
		PC.setDirY(y);
	}
}

void Drag(int x, int y) {
	if (GState == PLAY) {
		PC.setDirX(x);
		PC.setDirY(y);
	}
}