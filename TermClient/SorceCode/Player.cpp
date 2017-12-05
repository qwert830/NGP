#include "../Header/Player.h"



Player::Player()
{
}

Player::Player(const float & x, const float & y,
	const float & dx, const float & dy, const int& clr) :
	X(x), Y(y), DirX(dx), DirY(dy), status(LIVE), HP(100),
	color(clr)
{
}
Player::~Player()
{
}

void Player::init(float initx, float inity) {
	X = initx;
	Y = inity;
	DirX = X;
	DirY = Y;
	setCondition(100, LIVE);
}

void Player::setCondition(const int& hp, const int& sta) {
	HP = hp;
	status = sta;
}

float Player::getX() const {
	return X;
}
float Player::getY() const {
	return Y;
}
float Player::getDirX() const {
	return DirX;
}
float Player::getDirY() const {
	return DirY;
}

void Player::setX(const float& sx) {
	X = sx;
}
void Player::setY(const float& sy) {
	Y = sy;
}
void Player::setDirX(const float& sx) {
	DirX = sx;
}
void Player::setDirY(const float& sy) {
	DirY = sy;
}

int Player::getStatus() const {
	return status;
}
int Player::getHP() const {
	return HP;
}


void Player::draw() {
	if (status) {
		//HP¹Ù ¹ÙÅÁ(Èò»ö)
		glLineWidth(7);
		glColor3ub(255, 255, 255);
		glBegin(GL_LINES);
		glVertex2f(X - CharaSize - 2, Y - CharaSize - 10);
		glVertex2f(X + CharaSize + 2, Y - CharaSize - 10);
		glEnd();
		//HP¹Ù °ÔÀÌÁö(ÃÊ·Ï-³ë¶û-»¡°­)
		glLineWidth(5);
		if (HP >= 65)
			glColor3ub(0, 200, 0);
		else if (HP >= 30)
			glColor3ub(200, 200, 0);
		else
			glColor3ub(200, 0, 0);

		glBegin(GL_LINES);
		glVertex2f(X - CharaSize, Y - CharaSize - 10);
		glVertex2f(X - CharaSize + (((CharaSize + CharaSize) / 100) * HP), Y - CharaSize - 10);
		glEnd();
		glLineWidth(1);

		//Ä³¸¯ÅÍ(¿ø)
		glPointSize(2);
		switch (color) {
		case RED:
			glColor3ub(255, 0, 0);
			break;
		case GREEN:
			glColor3ub(0, 255, 0);
			break;
		}
		glBegin(GL_POINTS);
		for (int i = 0; i < 360; i += 6) {
			glVertex2f(X + (CharaSize * sin(i * M_PI / 180)), Y + (CharaSize * cos(i * M_PI / 180)));
		}
		glEnd();
		glPushMatrix();
		glTranslated(X, Y, 0);
		glRotatef(atan2f(DirY - Y, DirX - X) * 180 / M_PI, 0, 0, 1);
		glTranslated(-X, -Y, 0);
		glBegin(GL_LINES);
		glVertex2i(X + CharaSize / 2 + (CharaSize / 2 * sin(-30 * M_PI / 180)), Y + (CharaSize / 2 * cos(-30 * M_PI / 180)));
		glVertex2i(X + CharaSize / 2 + (CharaSize / 2 * sin(90 * M_PI / 180)), Y + (CharaSize / 2 * cos(90 * M_PI / 180)));
		glVertex2i(X + CharaSize / 2 + (CharaSize / 2 * sin(90 * M_PI / 180)), Y + (CharaSize / 2 * cos(90 * M_PI / 180)));
		glVertex2i(X + CharaSize / 2 + (CharaSize / 2 * sin(210 * M_PI / 180)), Y + (CharaSize / 2 * cos(210 * M_PI / 180)));
		glVertex2i(X + CharaSize / 2 + (CharaSize / 2 * sin(210 * M_PI / 180)), Y + (CharaSize / 2 * cos(210 * M_PI / 180)));
		glVertex2i(X + CharaSize / 2 + (CharaSize / 2 * sin(-30 * M_PI / 180)), Y + (CharaSize / 2 * cos(-30 * M_PI / 180)));
		glEnd();
		glPopMatrix();
	}
}