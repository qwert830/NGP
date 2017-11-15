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

// 소켓 함수 오류 출력 후 종료
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

// 소켓 함수 오류 출력
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

	// 윈속 초기화
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

	// 데이터 통신에 사용할 변수

	string name;
	cout << "읽어올 파일을 입력해 주세요." << endl;
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
		cout << "데이터의 길이 : " << len << endl;

		// 서버와 데이터 통신
		// 데이터 보내기(고정 길이)

		//string 이름 보내기
		retval = send(sock, name.c_str(), sizeof(string), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send() 고정1");
		}
		//int 데이터 길이 보내기
		retval = send(sock, (char *)&len, sizeof(int), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send() 고정2");
		}

		// 데이터 보내기(가변 길이)
		retval = 0;
		while (true)
		{
			if (retval + BUFSIZE <= len)
			{
				file.read(buf, BUFSIZE);
				tempval = send(sock, buf, BUFSIZE, 0);
				if (tempval == SOCKET_ERROR) {
					err_display("send() 가변");
				}
				retval += tempval;
			}
			else // 마지막 데이터
			{
				file.read(buf, len - retval);
				tempval = send(sock, buf, len - retval, 0);
				if (tempval == SOCKET_ERROR) {
					err_display("send() 가변");
				}
				retval += tempval;
				break;
			}
		}
		printf("[TCP 클라이언트] %d+%d+%d바이트를 "
			"보냈습니다.\n", sizeof(string), sizeof(int), retval);

		file.close();

	}
	// closesocket()
	closesocket(sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}