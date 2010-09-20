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

#if FACING_AS_FIXED
# define JET_ROTATE 8
#else
# define JET_ROTATE (M_PI_4/6)
#endif

#define JET_ACCEL  1.0

Jet::Jet(double speed, BITMAP *bmp, struct sockaddr_in *addr)

	: PhyObj(0, 0, speed, RAND_RAD(), bmp->w, bmp->h),
		NetObj(addr),
		_bmp(bmp),
		_col(makecol(0, 255, 0)),
		_thrust(false),
#if FACING_AS_FIXED
		_facing(ftofix(RAND_DEG())),
		sensitivity(itofix(JET_ROTATE))
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
	double ptx = _x, pty = _y;

#if FACING_AS_FIXED
	::rotate_sprite(buffer, _bmp, _x, _y, _facing);
#else
	::rotate_sprite(buffer, _bmp, _x, _y, radtofix(_facing));
#endif

	applyvector(&ptx, &pty, 50, _facing);

	// bmp, x1,y1, x2,y2, col
	line(buffer, _x, _y, ptx, pty, makecol(255,255,255));
}

void Jet::move(double xlim, double ylim)
{
	PhyObj::move(xlim, ylim); // superclass funcall
	if(_thrust){
#if FACING_AS_FIXED
		// TODO
#else
		::addvectors(&_speed, &_heading, JET_ACCEL, _facing);
#endif
	}
}

void Jet::thrust(bool on)
{
	_thrust = on;
}

void Jet::rotate_right()
{
#if FACING_AS_FIXED
	_facing += sensitivity;
#else
	_facing = clampangle(_facing + JET_ROTATE);
#endif
}

void Jet::rotate_left()
{
#if FACING_AS_FIXED
	_facing -= sensitivity;
#else
	_facing = clampangle(_facing - JET_ROTATE);
#endif
}
