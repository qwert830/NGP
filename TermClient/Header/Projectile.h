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

	void draw();
	
};