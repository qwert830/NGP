#pragma once

class Character
{
public:
	float x = 0, y = 0, dx = 0, dy = 0;
	int hp = 0, status = 0, atkCool = 0;
};

class Projectile
{
public:
	float x =0 , y = 0, dx = 0, dy = 0;
	int animation = 0;
};

