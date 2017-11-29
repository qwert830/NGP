#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
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


void err_quit(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}
void err_display(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}
int recvn(SOCKET s, char *buf, int len, int flags)
{
	int received;
	char *ptr = buf;
	int left = len;

	while (left > 0) {
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;
		left -= received;
		ptr += received;
	}

	return (len - left);
}

Player PC1(100, WindowHei / 2, 0, 0, GREEN), PC2(100, 700, 0, 0, RED);
list<Projectile> p;

bool RDragCheck, LDragCheck;
int GState;



#define SERVERPORT 9000
#define BUFSIZE    1024

struct ServerAction {
	int id, hp, status;
	float x, y, dx, dy;
	Projectile projectiles[20];
	int GameState;

	ServerAction() :id(0), hp(0), status(0), x(0), y(0), dx(0), dy(0) {}
};

struct ClientAction {
	float mx, my;
	bool leftClick, rightClick;
};

char buf[BUFSIZE];

int retval;
WSADATA wsa;
SOCKET sock;
ServerAction SA1, SA2;
bool is_start = false;

void GetRecvInfo(SOCKET s, char* buf, ServerAction& sa) {
	recvn(s, buf, sizeof(ServerAction), 0);
	memcpy(&sa, buf, sizeof(ServerAction));
}


void main(int argc, char *argv[]) {
	char SERVERIP[20];
	cout << "IP주소 : ";
	cin >> SERVERIP;

	// 윈속 초기화
	
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return;

	// socket()
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");

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

	// closesocket()
	closesocket(sock);
	// 윈속 종료
	WSACleanup();
}

GLvoid drawScene(GLvoid) {
	glClearColor(0.0, 0.0, 0.0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	switch (GState)
	{
	case TITLE:
	{
		char title[2][25]{ { "VECTOR RUSH" },{ "WAIT FOR THE GAME START" }};
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
		is_start = true;
	}
	break;
	case PLAY:
		PC1.draw();
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
	case TITLE:
		if (is_start)
		{
			GetRecvInfo(sock, buf, SA1);
			GetRecvInfo(sock, buf, SA2);
			RDragCheck = false;
			LDragCheck = false;
			if (SA1.GameState == 1 && SA2.GameState == 1)
				GState = PLAY;
		}
		break;
	case PLAY:
		PC1.update(0.01, LDragCheck, RDragCheck);

		if (PC1.getStatus() == DEAD) {
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
			PC1.init(100, WindowHei / 2);
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

}

void Drag(int x, int y) {

}