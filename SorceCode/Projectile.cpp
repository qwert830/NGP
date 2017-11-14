#include "../Header/Projectile.h"


Projectile::Projectile(const float& a, const float& b, const float& c, const float& d)
	: X(a), Y(b), DirX(c), DirY(d), TempX(a), TempY(b),
	speed(3), animation(0), damage(10) {
	TempX = cos(atan2f(DirY - TempY, DirX - TempX));
	TempY = sin(atan2f(DirY - TempY, DirX - TempX));
}

int Projectile::getDmg() {
	return damage;
}

void Projectile::move() {
	X +=  TempX* speed;
	Y +=  TempY* speed;
}
void Projectile::draw() {

	glBegin(GL_POLYGON);
	for (int i = 0; i < 360; i += 36) {
		glVertex2i(X + (ProjSize * sin(i * M_PI / 180)), Y + (ProjSize * cos(i * M_PI / 180)));
	}
	glEnd();
	glPushMatrix();
	glTranslated(X, Y, 0);
	glRotatef(atan2f(DirY - TempY, DirX - TempX) * 180 / M_PI, 0, 0, 1);
	glRotatef(animation = animation + 6 % 360, 1, 0, 0);
	glTranslated(-X, -Y, 0);
	glBegin(GL_LINES);
	glVertex2i(X + ProjSize + (ProjSize * 2 * sin(-30 * M_PI / 180)), Y + (ProjSize * 2 * cos(-30 * M_PI / 180)));
	glVertex2i(X + ProjSize + (ProjSize * 2 * sin(90 * M_PI / 180)), Y + (ProjSize * 2 * cos(90 * M_PI / 180)));
	glVertex2i(X + ProjSize + (ProjSize * 2 * sin(90 * M_PI / 180)), Y + (ProjSize * 2 * cos(90 * M_PI / 180)));
	glVertex2i(X + ProjSize + (ProjSize * 2 * sin(210 * M_PI / 180)), Y + (ProjSize * 2 * cos(210 * M_PI / 180)));
	glVertex2i(X + ProjSize + (ProjSize * 2 * sin(210 * M_PI / 180)), Y + (ProjSize * 2 * cos(210 * M_PI / 180)));
	glVertex2i(X + ProjSize + (ProjSize * 2 * sin(-30 * M_PI / 180)), Y + (ProjSize * 2 * cos(-30 * M_PI / 180)));
	glEnd();
	glPopMatrix();

}