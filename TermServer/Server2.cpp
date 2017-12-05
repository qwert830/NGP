#include "Server2.h"
#include <list>

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

void CreateBullet(Character* player, list<Projectile>* bullet, int id)
{

	

	
}


void CollisionCheck(Character* player, list<Projectile>* bullet)
{

	for (int i = 0; i < MAX_CLIENT; i++)
	{
		for (auto d = bullet[1 - i].begin(); d != bullet[1-i].end(); ++d)
		{
			if (sqrt(((player[i].x - d->x)*(player[i].x - d->x)) + ((player[i].y - d->y)*(player[i].y - d->y))) <= CHARACTER_SIZE + PROJECTILE_SIZE)
			{
				player[i].hp -= 5;
				d = bullet[1 - i].erase(d);
			}
			if (d == bullet[1 - i].end())
				break;
			if (d->x > WINDOW_W || d->x <0 || d->y > WINDOW_H || d->y < 0)
			{
				d = bullet[1 - i].erase(d);
			}
			if (d == bullet[1 - i].end())
				break;

		}
	}

}