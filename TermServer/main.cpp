#include "Base.h"
#include "Server1.h"
#include "Server2.h"


#define SERVERPORT 9000

using namespace std;

list<HANDLE> clientThread;
HANDLE updateThread;
HANDLE clientEvent[MAX_CLIENT];
HANDLE updateEvent;

int gameStatus = 0; // 0 대기상태 1 게임상태 2 게임종료
int playerID = 0;
int accessPlayerCount = 0;
Character Player[MAX_CLIENT];
list<Projectile> projList[MAX_CLIENT];

auto preT = chrono::system_clock::now();
auto curT = chrono::system_clock::now();

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
DWORD WINAPI UpdateThread(LPVOID arg);
DWORD WINAPI ClientThread(LPVOID arg);

DWORD WINAPI UpdateThread(LPVOID arg)
{	
	DWORD retval;
	preT = curT = chrono::system_clock::now();

	ServerInit(Player);
	gameStatus = 1;
	
	SetEvent(updateEvent);

	while (1)
	{
		curT = chrono::system_clock::now();
		chrono::duration<double> elapsedTime = (curT - preT);
		preT = curT;
<<<<<<< HEAD
		retval = WaitForMultipleObjects(MAX_CLIENT, clientEvent, TRUE, INFINITE);
		if (retval != WAIT_OBJECT_0) break;
=======

>>>>>>> 2b6c5518eb242872d5dc6df672fe18c39b7ff00a
		ResetEvent(clientEvent[0]);
		ResetEvent(clientEvent[1]);

		for (int i = 0; i < MAX_CLIENT; i++)
		{
			//이동부분
			Player[i].atkCool += elapsedTime.count();
<<<<<<< HEAD
=======
			cout << elapsedTime.count()<<endl;
>>>>>>> 2b6c5518eb242872d5dc6df672fe18c39b7ff00a
			if (Player[i].leftClick)
			{
				Player[i].x += cos(atan2f(Player[i].dy - Player[i].y, Player[i].dx - Player[i].x)) * PLAYER_SPEED * elapsedTime.count();
				Player[i].y += sin(atan2f(Player[i].dy - Player[i].y, Player[i].dx - Player[i].x)) * PLAYER_SPEED * elapsedTime.count();
				if (Player[i].x > WINDOW_W)
				{
					Player[i].x = WINDOW_W;
				}
				else if (Player[i].x < 0)
				{
					Player[i].x = 0;
				}

				if (Player[i].y > WINDOW_H)
				{
					Player[i].y = WINDOW_H;
				}
				else if (Player[i].y < 0)
				{
					Player[i].y = 0;
				}
			}
<<<<<<< HEAD
			for(int j = 0; j<MAX_CLIENT; ++j)
				for (auto d = projList[j].begin(); d !=projList[j].end(); ++d)
				{
					d->x += cos(atan2f(d->dy, d->dx)) * BULLET_SPEED * elapsedTime.count();
					d->y += sin(atan2f(d->dy, d->dx)) * BULLET_SPEED * elapsedTime.count();
				}
=======
			for (auto d = projList[i].begin(); d != projList[i].end(); ++d)
			{
				d->x += cos(atan2f(d->dy, d->dx)) * BULLET_SPEED* elapsedTime.count();
				d->y += sin(atan2f(d->dy, d->dx)) * BULLET_SPEED* elapsedTime.count();
			}
>>>>>>> 2b6c5518eb242872d5dc6df672fe18c39b7ff00a
			if (Player[i].rightClick && Player[i].atkCool >= 0.5)
			{
				CreateBullet(Player, projList, i);
				Player[i].atkCool = 0;
			}
		}
		CollisionCheck(Player,projList);

		SetEvent(updateEvent);
		if (accessPlayerCount < MAX_CLIENT)
			break;
	}

	return 0;
}

DWORD WINAPI ClientThread(LPVOID arg)
{
	SOCKET client_sock = (SOCKET)arg;
	ClientAction CA;
	ServerAction SA;
	DWORD retval;
	char buf[BUFSIZE + 1];
	int id = playerID++;
	ZeroMemory(buf, BUFSIZE);
	
	WaitForSingleObject(updateEvent, INFINITE);

	// 기본 데이터 전송
	CreateData(SA, Player, projList, 0, gameStatus);
	send(client_sock, (char*)&SA, sizeof(ServerAction), 0);
	CreateData(SA, Player, projList, 1, gameStatus);
	send(client_sock, (char*)&SA, sizeof(ServerAction), 0);
	
	ResetEvent(updateEvent);
	//통신부분
	while (1)
	{
<<<<<<< HEAD
		retval = recvn(client_sock, buf, sizeof(ClientAction), 0);
		if (retval == SOCKET_ERROR)
			break;
		memcpy(&CA, buf, sizeof(ClientAction));
		ZeroMemory(buf, BUFSIZE);
=======
		retval = recvn(client_sock, (char*)&CA, sizeof(ClientAction), 0);
		if (retval == SOCKET_ERROR)
			break;
>>>>>>> 2b6c5518eb242872d5dc6df672fe18c39b7ff00a
		Decoding(CA, Player, id);
		SetEvent(clientEvent[id]);

		WaitForSingleObject(updateEvent, INFINITE);
		ResetEvent(updateEvent);

		// 데이터 전송
		CreateData(SA, Player, projList, 0, gameStatus);
		retval = send(client_sock, (char*)&SA, sizeof(ServerAction), 0);
		if (retval == SOCKET_ERROR)
			break;
		CreateData(SA, Player, projList, 1, gameStatus);
		retval = send(client_sock, (char*)&SA, sizeof(ServerAction), 0);
		if (retval == SOCKET_ERROR)
			break;

		if (gameStatus == 2)
			break;
	}
<<<<<<< HEAD
=======

>>>>>>> 2b6c5518eb242872d5dc6df672fe18c39b7ff00a
	closesocket(client_sock);
	if (playerID > id)
	{
		playerID = id;
	}
	accessPlayerCount--;
<<<<<<< HEAD
=======
	gameStatus = 2;

>>>>>>> 2b6c5518eb242872d5dc6df672fe18c39b7ff00a
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

	clientEvent[0] = CreateEvent(NULL, TRUE, FALSE, NULL);
	clientEvent[1] = CreateEvent(NULL, TRUE, FALSE, NULL);
	updateEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

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

		if (accessPlayerCount < MAX_CLIENT) // 최대 클라이언트 제한
		{
			clientThread.push_back(new HANDLE(CreateThread(NULL, 0, ClientThread, (LPVOID)client_sock, 0, NULL)));
			accessPlayerCount += 1;
			if (clientThread.back() == NULL) { closesocket(client_sock); }
		}
		if (accessPlayerCount >= MAX_CLIENT)
			updateThread = CreateThread(NULL, 0, UpdateThread, NULL, 0, NULL);
<<<<<<< HEAD
=======
		if (accessPlayerCount == 0)
		{
			clientThread.clear();
		}
>>>>>>> 2b6c5518eb242872d5dc6df672fe18c39b7ff00a
	}
	// closesocket()
	closesocket(listen_sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}

