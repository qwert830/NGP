#include "../Header/Projectile.h"

Projectile::Projectile()
	: x(0), y(0), dx(0), dy(0), animation(0)
{
}

Projectile::Projectile(const float& a, const float& b, const float& c, const float& d)
	: x(a), y(b), dx(c), dy(d), animation(0) 
{
}

float Projectile::getX()const {
	return x;
}
float Projectile::getDX()const {
	return dx;
}
float Projectile::getY()const {
	return y;
}
float Projectile::getDY()const {
	return dy;
}

bool Projectile::getmDraw()const {
	return mdraw;
}

void Projectile::setmDraw(bool smd) {
	mdraw = smd;
}

void Projectile::setX(const float sx) {
	x = sx;
}
void Projectile::setDX(const float sdx) {
	dx = sdx;
}
void Projectile::setY(const float sy) {
	y = sy;
}
void Projectile::setDY(const float sdy) {
	dy = sdy;
}

void Projectile::draw() {
	if (mdraw) {
		glBegin(GL_POLYGON);
		for (int i = 0; i < 360; i += 36) {
			glVertex2i(x + (ProjSize * sin(i * M_PI / 180)), y + (ProjSize * cos(i * M_PI / 180)));
		}
		glEnd();
		glPushMatrix();
		glTranslated(x, y, 0);
		glRotatef(atan2f(dy, dx) * 180 / M_PI, 0, 0, 1);
		glRotatef(animation = (animation + 6) % 360, 1, 0, 0);
		glTranslated(-x, -y, 0);
		glBegin(GL_LINES);
		glVertex2i(x + ProjSize + (ProjSize * 2 * sin(-30 * M_PI / 180)), y + (ProjSize * 2 * cos(-30 * M_PI / 180)));
		glVertex2i(x + ProjSize + (ProjSize * 2 * sin(90 * M_PI / 180)), y + (ProjSize * 2 * cos(90 * M_PI / 180)));
		glVertex2i(x + ProjSize + (ProjSize * 2 * sin(90 * M_PI / 180)), y + (ProjSize * 2 * cos(90 * M_PI / 180)));
		glVertex2i(x + ProjSize + (ProjSize * 2 * sin(210 * M_PI / 180)), y + (ProjSize * 2 * cos(210 * M_PI / 180)));
		glVertex2i(x + ProjSize + (ProjSize * 2 * sin(210 * M_PI / 180)), y + (ProjSize * 2 * cos(210 * M_PI / 180)));
		glVertex2i(x + ProjSize + (ProjSize * 2 * sin(-30 * M_PI / 180)), y + (ProjSize * 2 * cos(-30 * M_PI / 180)));
		glEnd();
		glPopMatrix();
	}
}