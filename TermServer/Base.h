#pragma once

#pragma comment(lib, "ws2_32")

#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <list>
#include <chrono>

using namespace std;

#define BUFSIZE    1000
#define MAX_CLIENT 2

#define PLAYER_SPEED 200
#define BULLET_SPEED 200

#define WINDOW_W 800
#define WINDOW_H 600
#define CHARACTER_SIZE 12.0
#define PROJECTILE_SIZE 6.0
#define DAMAGE 5