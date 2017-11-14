#pragma once
#include <glut.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include "DEFINE.h"
#include <list>
using namespace std;

class Projectile
{
	float X, Y, DirX, DirY;
	float TempX, TempY, speed;
	int animation, damage;
public:
	Projectile(const float& a, const float& b, const float& c, const float& d);

	int getDmg();

	void move();

	void draw();
};