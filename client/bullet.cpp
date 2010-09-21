#include <allegro.h>

#include "phyobj.h"

#include "bullet.h"

#define BULLET_RADIUS 5

Bullet::Bullet(double x, double y, double speed, double heading, int col)
	: PhyObj(x, y, speed, heading, BULLET_RADIUS, BULLET_RADIUS), _col(col)
{
}

Bullet::~Bullet()
{
}

void Bullet::draw(BITMAP *bmp) const
{
	circlefill(bmp, _x, _y, BULLET_RADIUS, _col);
}
