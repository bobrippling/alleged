#include <allegro.h>

#include "phyobj.h"
#include "time.h"

#include "bullet.h"

#define BULLET_RADIUS  5
#define BULLET_TIMEOUT 1500

Bullet::Bullet(double x, double y, double speed, double heading, int col)
	: PhyObj(x, y, speed, heading, BULLET_RADIUS, BULLET_RADIUS),
		_col(col),
		_maketime(time_now())
{
}

Bullet::~Bullet()
{
}

void Bullet::draw(BITMAP *bmp) const
{
	circlefill(bmp, _x, _y, BULLET_RADIUS, _col);
}

bool Bullet::expired() const
{
	return time_now() - _maketime > BULLET_TIMEOUT;
}
