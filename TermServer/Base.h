#pragma once

#pragma comment(lib, "ws2_32")

#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <list>
#include <chrono>

using namespace std;

#define BUFSIZE    1000
#define MAX_CLIENT 2

#define PLAYER_SPEED 200
#define ATK_SPEED 100
#define BULLET_SPEED 150

#define WINDOW_W 1200
#define WINDOW_H 800
#define CHARACTER_SIZE 14.0
#define PROJECTILE_SIZE 7.0
#define DAMAGE 12