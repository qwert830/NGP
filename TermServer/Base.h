#pragma once

#pragma comment(lib, "ws2_32")

#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <list>

#define BUFSIZE    1000
#define MAX_CLIENT 2
#define PLAYER_SPEED 6
#define WINDOW_W 800
#define WINDOW_H 600