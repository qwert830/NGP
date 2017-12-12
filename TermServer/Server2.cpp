#include "Server2.h"

void ServerInit(Character* player, list<Projectile>* bullet)
{
	for (int i = 0; i < MAX_CLIENT; i++)
	{
		player[i].atkCool = 0;
		player[i].dx = 400;
		player[i].dy = 300;
		player[i].hp = 100;
		player[i].leftClick = false;
		player[i].rightClick = false;
		player[i].status = 1;
		player[i].y = WINDOW_H / 2;
	}
	player[0].x = WINDOW_W / 6;
	player[1].x = WINDOW_W - (WINDOW_W / 6);
	
	bullet[0].clear();
	bullet[1].clear();
}

void CreateBullet(Character* player, list<Projectile>* bullet, int id)
{
	bullet[id].push_back(Projectile(player[id].x,player[id].y,player[id].dx-player[id].x,player[id].dy-player[id].y));
}

void CollisionCheck(Character* player, list<Projectile>* bullet)
{
	for (int i = 0; i < MAX_CLIENT; i++)
	{
		for (auto d = bullet[1 - i].begin(); d != bullet[1-i].end(); ++d)
		{
			if (sqrt(((player[i].x - d->x)*(player[i].x - d->x)) + ((player[i].y - d->y)*(player[i].y - d->y))) <= CHARACTER_SIZE + PROJECTILE_SIZE)
			{
				player[i].hp -= DAMAGE;
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