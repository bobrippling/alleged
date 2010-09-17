#include <cmath>

#include <allegro.h>
#include <arpa/inet.h>

#include "phyobj.h"
#include "netobj.h"

#include "jet.h"

Jet::Jet(double w, double h, double speed,
		BITMAP *bmp, struct sockaddr_in *addr)
	: PhyObj(rand() * w, rand() * h, speed, (rand() % 360) * M_PI/180, w, h),
	NetObj(addr),
	_bmp(bmp), _col(makecol(0, 255, 0))
{
}

Jet::Jet(double x, double y, double speed, double heading,
		BITMAP *bmp, int col,
		struct sockaddr_in *addr)
	: PhyObj(x, y, speed, heading, bmp->w, bmp->h),
	NetObj(addr),
	_bmp(bmp), _col(col)
{
}

Jet::~Jet()
{
	destroy_bitmap(_bmp);
}

void Jet::draw(::BITMAP *buffer)
{
	::draw_sprite(buffer, _bmp, _x, _y);
	::textprintf_centre_ex(buffer, font, _x, _y, _col, -1, "jetTODO");
	/* bmp, font, x, y, col, transparency, str, ... */
}
