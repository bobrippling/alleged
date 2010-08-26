#include <allegro.h>

#include "phyobj.h"
#include "ball.h"

Ball::Ball(double x, double y, double speed, double heading, BITMAP *bmp, int col)
	: PhyObj(x, y, speed, heading, bmp->w, bmp->h), _bmp(bmp), _col(col)
{
}

Ball::~Ball()
{
	destroy_bitmap(_bmp);
}
