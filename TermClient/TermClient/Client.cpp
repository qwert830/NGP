#pragma comment(lib, "ws2_32")
#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <iostream>
#include <string>

#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE    100000

using namespace std;

// ���� �Լ� ���� ��� �� ����
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

// ���� �Լ� ���� ���
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

int main(int argc, char *argv[])
{
	int retval;

	// ���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");

	// ������ ��ſ� ����� ����

	string name;
	cout << "�о�� ������ �Է��� �ּ���." << endl;
	cin >> name;

	fstream file;

	file.open(name, ios::in | ios::binary);

	if (file.is_open())
	{
		char buf[BUFSIZE + 1];

		memset(buf, 0, sizeof(buf));

		file.seekg(0, ios::end);
		int len = file.tellg();
		int tempval = 0;
		file.seekg(0, ios::beg);
		cout << "�������� ���� : " << len << endl;

		// ������ ������ ���
		// ������ ������(���� ����)

		//string �̸� ������
		retval = send(sock, name.c_str(), sizeof(string), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send() ����1");
		}
		//int ������ ���� ������
		retval = send(sock, (char *)&len, sizeof(int), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send() ����2");
		}

		// ������ ������(���� ����)
		retval = 0;
		while (true)
		{
			if (retval + BUFSIZE <= len)
			{
				file.read(buf, BUFSIZE);
				tempval = send(sock, buf, BUFSIZE, 0);
				if (tempval == SOCKET_ERROR) {
					err_display("send() ����");
				}
				retval += tempval;
			}
			else // ������ ������
			{
				file.read(buf, len - retval);
				tempval = send(sock, buf, len - retval, 0);
				if (tempval == SOCKET_ERROR) {
					err_display("send() ����");
				}
				retval += tempval;
				break;
			}
		}
		printf("[TCP Ŭ���̾�Ʈ] %d+%d+%d����Ʈ�� "
			"���½��ϴ�.\n", sizeof(string), sizeof(int), retval);

		file.close();

	}
	// closesocket()
	closesocket(sock);

	// ���� ����
	WSACleanup();
	return 0;
}