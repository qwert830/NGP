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
	Character* player;
	Projectile* projectile;
	
	for (int i = 0; i < MAX_CLIENT; i++)
	{
		for (int j = 0; i < 20; i++)
		{
			if (projectile[j].x - player[i].x < 5 && projectile[j].x - player[i].x > -5 && projectile[j].y - player[i].y < 5 && projectile[j].y - player[i].y > -5)
			{
				player[i].hp -= 15;
			}
		}
	}

}