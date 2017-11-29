#include "Server2.h"

void ServerInit(Character* player)
{
	for (int i = 0; i < MAX_CLIENT; i++)
	{
		player[i].atkCool = 0;
		player[i].dx = 0;
		player[i].dy = 0;
		player[i].hp = 100;
		player[i].leftClick = false;
		player[i].rightClick = false;
		player[i].status = 1;
		player[i].y = 300;
	}
	player[0].x = 100;
	player[1].x = 700;
}

void CreateBullet()
{
}

void CollisionCheck()
{
}