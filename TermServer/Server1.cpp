#include "Server1.h"

void Decoding(ClientAction& CA, Character* Player, int id)
{
	Player[id].dx = CA.mx;
	Player[id].dy = CA.my;
	Player[id].leftClick = CA.leftClick;
	Player[id].rightClick = CA.rightClick;
}

void CreateData(ServerAction& SA, Character* Player, list<Projectile>* projList, int id, int gameStatus)
{
	SA.dx = Player[id].dx;
	SA.dy = Player[id].dy;
	SA.hp = Player[id].hp;
	SA.id = id;
	SA.status = Player[id].status;
	SA.x = Player[id].x;
	SA.y = Player[id].y;
	SA.GameState = gameStatus;
	int i = 0;
	for (int n = 0; n < 20; n++)
	{
		SA.projectiles[n].animation = 0;
		SA.projectiles[n].x = 0;
		SA.projectiles[n].y = 0;
		SA.projectiles[n].dx = 0;
		SA.projectiles[n].dy = 0;
		SA.projectiles[n].draw = false;
	}
	for (auto d : projList[id])
	{
		SA.projectiles[i++] = d;
	}
}