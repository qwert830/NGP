#include "Base.h"
#include "Struct.h"
#include "ServerClass.h"

#define SERVERPORT 9000

using namespace std;

list<HANDLE> clientThread;
int gameStatus = 0; // 0 대기상태 1 게임상태 2 게임종료
int playerID = 0;
Character Player[MAX_CLIENT];

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

// 사용자 정의 데이터 수신 함수
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

void Decoding();
void CreateData();
void ServerInit();
void CreateBullet();
void CollisionCheck();

DWORD WINAPI UpdateThread(LPVOID arg);
DWORD WINAPI ClientThread(LPVOID arg);

DWORD WINAPI UpdateThread(LPVOID arg)
{	
	return 0;
}

DWORD WINAPI ClientThread(LPVOID arg)
{
	SOCKET client_sock = (SOCKET)arg;
	ClientAction CA;
	ServerAction SA;
	int retval;
	char buf[BUFSIZE + 1];
	int id = playerID++;
	ZeroMemory(buf, BUFSIZE);
	/*

	업데이트 함수로

	if (clientThread.size() == MAX_CLIENT)
	{
		send(client_sock, (char*)gameStatus, sizeof(int), 0);
		send(client_sock, (char*)Player, sizeof(Character), 0);
	}
	*/


	//통신부분
	while (1)
	{
		recvn(client_sock, buf, BUFSIZE, 0);
		memcpy(&CA, buf, sizeof(ClientAction));
		ZeroMemory(buf, BUFSIZE);
		Player[id].dx = CA.mx;
		Player[id].dy = CA.my;
		Player[id].leftClick = CA.leftClick;
		Player[id].rightClick = CA.rightClick;
		/* 
			이벤트 처리
		*/

		// 업데이트
		
		// 데이터 전송

	}
	return 0;
}

int main(int argc, char *argv[])
{
	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");

	// bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");

	// 데이터 통신에 사용할 변수
	SOCKET client_sock;
	SOCKADDR_IN clientaddr;
	int addrlen;

	while (1)
	{
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_display("accept()");
			break;
		}
		// 접속한 클라이언트 정보 출력
		printf("\n[TCP 서버] 클라이언트 접속: IP 주소=%s, 포트 번호=%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		if (clientThread.size() < MAX_CLIENT) // 최대 클라이언트 제한
		{
			clientThread.push_back(new HANDLE(CreateThread(NULL, 0, ClientThread, (LPVOID)client_sock, 0, NULL)));
			if (clientThread.back() == NULL) { closesocket(client_sock); }
		}
	}
	printf("[TCP 서버] 클라이언트 종료: IP 주소=%s, 포트 번호=%d\n",
		inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
	// closesocket()
	closesocket(listen_sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}