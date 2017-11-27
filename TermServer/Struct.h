#pragma once
#include "ServerClass.h"

struct ServerAction {
	int id, hp, status;
	float x, y, dx, dy;
	Projectile projectiles[20];
	int GameState;

	ServerAction() :id(0), hp(0), status(0), x(0), y(0), dx(0), dy(0) {}
};

struct ClientAction {
	float mx, my;
	bool leftClick, rightClick;

	ClientAction() { mx = 0; my = 0; leftClick = false; rightClick = false; }
};