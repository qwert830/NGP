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
#include <string>
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
Projectile p1[20], p2[20];

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

	ClientAction() : mx(0), my(0), leftClick(false), rightClick(false) {}
};

char buf[BUFSIZE];

int retval;
WSADATA wsa;
SOCKET sock;
ServerAction SA1, SA2;
ClientAction CA;
bool is_start = false;

void GetRecvInfo(SOCKET s, char* buf, ServerAction& sa) {
	recvn(s, buf, sizeof(ServerAction), 0);
	memcpy(&sa, buf, sizeof(ServerAction));
}

void Decoding(Player& pc, ServerAction& sa, Projectile* p) {
	pc.setDirX(sa.dx);
	pc.setDirY(sa.dy);
	pc.setX(sa.x);
	pc.setY(sa.y);
	pc.setCondition(sa.hp, sa.status);
	for (int i = 0; i < 20; ++i) {
		p[i].setX(sa.projectiles[i].getX());
		p[i].setDX(sa.projectiles[i].getDX());
		p[i].setY(sa.projectiles[i].getY());
		p[i].setDY(sa.projectiles[i].getDY());
		p[i].setmDraw(sa.projectiles[i].getmDraw());
	}
}

void main(int argc, char *argv[]) {
	// 윈속 초기화
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return;
	// socket()
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	char SERVERIP[20];
	SOCKADDR_IN serveraddr;

	while (1) {
		cout << "IP주소 : ";
		cin >> SERVERIP;
		// connect()
		ZeroMemory(&serveraddr, sizeof(serveraddr));
		serveraddr.sin_family = AF_INET;
		serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
		serveraddr.sin_port = htons(SERVERPORT);
		retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
		if (retval == SOCKET_ERROR) {
			err_display("connect()");
		}
		else
			break;
	}


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
		char title1[] = "VECTOR RUSH";
		char title2[] = "WAIT FOR THE GAME START";
		glColor3ub(0, 255, 0);
		glRasterPos2f(WindowWid / 2 - 100, WindowHei / 2 - 50);
		for (char *c = title1; *c != '\0'; ++c)
		{
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
		}
		glRasterPos2f(WindowWid / 2 - 100, WindowHei / 2 + 50);
		for (char *c = title2; *c != '\0'; ++c)
		{
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
		}
		is_start = true;
	}
	break;
	case PLAY:
		PC1.draw();
		PC2.draw();
		for (int i = 0; i < 20; ++i) {
			glColor3ub(0, 255, 0);
			p1[i].draw();
			glColor3ub(255, 0, 0);
			p2[i].draw();
		}
		break;
	case END:
		char end1[] = "GAME OVER";
		char end2[] = "GREEN WIN";
		char end3[] = "RED WIN";

		glColor3ub(0, 255, 0);
		glRasterPos2f(WindowWid / 2 - 100, WindowHei / 2 - 50);
		for (char *c = end1; *c != '\0'; ++c)
		{
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
		}
		if (PC1.getHP() > PC2.getHP()) {
			glRasterPos2f(WindowWid / 2 - 100, WindowHei / 2 + 50);
			for (char *c = end2; *c != '\0'; ++c)
			{
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
			}
		}
		else {
			glRasterPos2f(WindowWid / 2 - 100, WindowHei / 2 + 50);
			for (char *c = end3; *c != '\0'; ++c)
			{
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *c);
			}
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
			recvn(sock, (char*)&SA1, sizeof(ServerAction), 0);
			recvn(sock, (char*)&SA2, sizeof(ServerAction), 0);
			Decoding(PC1, SA1, p1);
			Decoding(PC2, SA2, p2);
		}
		break;
	case PLAY:
		recvn(sock, (char*)&SA1, sizeof(ServerAction), 0);
		recvn(sock, (char*)&SA2, sizeof(ServerAction), 0);
		Decoding(PC1, SA1, p1);
		Decoding(PC2, SA2, p2);
		break;
	case END:
		recvn(sock, (char*)&SA1, sizeof(ServerAction), 0);
		recvn(sock, (char*)&SA2, sizeof(ServerAction), 0);
		Decoding(PC1, SA1, p1);
		Decoding(PC2, SA2, p2);
		break;
	}
	GState = SA1.GameState;
	glutPostRedisplay();
	glutTimerFunc(GameSpd, TimerFunction, 1);
}

void Keyboard(unsigned char key, int x, int y) {

}

void Mouse(int button, int state, int x, int y) {
	if (GState == PLAY) {
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
			CA.leftClick = true;
		}
		else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
			CA.leftClick = false;
		}
		if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
			CA.rightClick = true;
		}
		else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
			CA.rightClick = false;
		}
		send(sock, (char*)&CA, sizeof(ClientAction), 0);
	}
}

void MousePos(int x, int y) {
	if (GState == PLAY) {
		CA.mx = x;
		CA.my = y;
		send(sock, (char*)&CA, sizeof(ClientAction), 0);
	}
}

void Drag(int x, int y) {
	if (GState == PLAY) {
		CA.mx = x;
		CA.my = y;
		send(sock, (char*)&CA, sizeof(ClientAction), 0);
	}
}