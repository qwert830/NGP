#pragma once
#include "../glut/glut.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "DEFINE.h"
#include <list>
using namespace std;

class Projectile
{
	float x, y, dx, dy;
	int animation;

	bool mdraw;

public:
	Projectile();
	Projectile(const float& a, const float& b, const float& c, const float& d);

	float getX() const;

	float getDX() const;

	float getY() const;

	float getDY() const;

	bool getmDraw() const;

	void setmDraw(bool smd);

	void setX(const float sx);

	void setDX(const float sdx);

	void setY(const float sy);

	void setDY(const float sdy);

	void draw();
	
};