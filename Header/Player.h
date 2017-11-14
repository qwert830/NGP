#pragma once
#include <glut.h>
#include <list>
#define _USE_MATH_DEFINES
#include <math.h>
#include "DEFINE.h"
#include "Projectile.h"
#include <algorithm>

class Player
{
protected:
	float X, Y, DirX, DirY;
	int speed, status, HP, atkCool, color;
public:
	Player();

	Player(const float & x, const float & y, const float & dx, const float & dy, const int & clr);
	~Player();
	void init(float initx, float inity);
	void setCondition(const int & hp, const int & sta, const int & cdn);
	int getStatus() const;
	int getHP() const;
	int getAtkCool() const;
	void draw();
	void update(const bool Lclk, const bool Rclk);
	void move();
	void attack();
	void hitChk(const int chkdmg);
};

