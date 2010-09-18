#include <cstdio>
#include <cstdlib>
#include <cmath>

#include <allegro.h>
#include <arpa/inet.h>

#include "phyobj.h"
#include "netobj.h"

#include "vector.h"

#include "jet.h"

#define TO_DEG(x)    ((x) * 180/M_PI)
#define TO_RAD(x)    ((x) * M_PI/180)

#define RAND_DEG()   (::rand() % 359)
#define RAND_RAD()   TO_RAD(RAND_DEG())

#define JET_ROTATE (M_PI_4/6)
#define JET_ACCEL  1.0

Jet::Jet(double speed, BITMAP *bmp, struct sockaddr_in *addr)

	: PhyObj(0, 0, speed, RAND_RAD(), bmp->w, bmp->h),
		NetObj(addr),
		_bmp(bmp),
		_col(makecol(0, 255, 0)),
		_thrust(false),
#if FACING_AS_FIXED
		_facing(ftofix(RAND_DEG())),
		sensitivity(itofix(8))
#else
		_facing(RAND_RAD())
#endif
{
}

Jet::Jet(
		double x,     double y,
		double speed, double heading,
		double facing,
		BITMAP *bmp,
		int col,
		struct sockaddr_in *addr)

	: PhyObj(x, y, speed, heading, bmp->w, bmp->h),
		NetObj(addr),
		_bmp(bmp),
		_col(col),
		_thrust(false),
#if FACING_AS_FIXED
		_facing(ftofix(TO_DEG(facing))),
		sensitivity(itofix(8))
#else
		_facing(facing)
#endif
{
}

Jet::~Jet()
{
	destroy_bitmap(_bmp);
}

void Jet::draw(::BITMAP *buffer)
{
#if FACING_AS_FIXED
	::rotate_sprite(buffer, _bmp, _x, _y, _facing);
#else
	::rotate_sprite(buffer, _bmp, _x, _y, ftofix(TO_DEG(_facing)));
#endif
}

void Jet::move(double xlim, double ylim)
{
	PhyObj::move(xlim, ylim); // superclass funcall
	if(_thrust){
		// TODO
#if FACING_AS_FIXED
#else
		//::applyvector(&_x, &_y, 1, _facing);
		::addvectors(&_speed, &_heading, JET_ACCEL, _facing);
#endif
	}
}

void Jet::thrust(bool on)
{
	_thrust = on;
}

#define P() printf("facing: %f\n", TO_DEG(_facing))

void Jet::rotate_right()
{
#if FACING_AS_FIXED
	_facing += sensitivity;
#else
	_facing = clipangle(_facing + JET_ROTATE);
	P();
#endif
}

void Jet::rotate_left()
{
#if FACING_AS_FIXED
	_facing -= sensitivity;
#else
	_facing = clipangle(_facing - JET_ROTATE);
	P();
#endif
}
