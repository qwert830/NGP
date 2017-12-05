#pragma once
#include "Base.h"

class Character
{
public:
	float x = 0, y = 0, dx = 0, dy = 0, atkCool = 0;
	int hp = 0, status = 0;
	bool leftClick = false;
	bool rightClick = false;
};

class Projectile
{
public:
	float x, y, dx, dy;
	int animation;
	bool draw;

	Projectile() : x(0), y(0), dx(0), dy(0), animation(0), draw(false) {}
	Projectile(int px, int py, int pdx, int pdy) : x(px), y(py), dx(pdx), dy(pdy), draw(true) {}
};

void ServerInit(Character* player);
void CreateBullet(Character* player, list<Projectile>* bullet, int id);
void CollisionCheck(Character* player ,list<Projectile>* bullet);

