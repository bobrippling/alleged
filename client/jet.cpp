#include <cstdio>
#include <cstdlib>
#include <cmath>

#include <allegro.h>
#include <arpa/inet.h>

// class defs
#include "phyobj.h"
#include "netobj.h"

// helpers
#include "time.h"
#include "vector.h"

// more class defs
#include "bullet.h"
#include "jet.h"

#define TO_DEG(x)    ((x) * 180/M_PI)
#define TO_RAD(x)    ((x) * M_PI/180)

#define RAND_DEG()   (::rand() % 359)
#define RAND_RAD()   TO_RAD(RAND_DEG())

// TODO: class cfg
#define JET_ROTATE   (M_PI_4/6)
#define JET_ACCEL    0.02
#define BULLET_SPEED 0.04
#define SHOT_DELAY   80

Jet::Jet(double speed, BITMAP *bmp, struct sockaddr_in *addr)

	: PhyObj(0, 0, speed, RAND_RAD(), bmp->w, bmp->h),
		NetObj(addr),
		_bmp(bmp),
		_col(makecol(0, 255, 0)),
		_thrust(false),
		_facing(RAND_RAD()),
		lastshot(time_now())
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
		_facing(facing),
		lastshot(time_now())
{
}

Jet::~Jet()
{
	destroy_bitmap(_bmp);
}

void Jet::draw(::BITMAP *buffer) const
{
	double ptx = _x, pty = _y;

	::rotate_sprite(buffer, _bmp, _x, _y, radtofix(_facing));

	applyvector(&ptx, &pty, 50, _facing);

	// bmp, x1,y1, x2,y2, col
	line(buffer, _x, _y, ptx, pty, makecol(255,255,255));
}

void Jet::move(double xlim, double ylim)
{
	// called every frame
	//
	PhyObj::move(xlim, ylim); // superclass funcall

	if(_thrust)
		::addvectors(&_speed, &_heading, JET_ACCEL, _facing);
}

void Jet::thrust(bool on)
{
	_thrust = on;
}

void Jet::rotate_right()
{
	_facing = clampangle(_facing + JET_ROTATE);
}

void Jet::rotate_left()
{
	_facing = clampangle(_facing - JET_ROTATE);
}

bool Jet::canfire()
{
	long now = ::time_now();

	if(now - lastshot > SHOT_DELAY){
		lastshot = now;
		return true;
	}
	return false;
}

Bullet *Jet::createbullet()
{
	return new Bullet(_x, _y, BULLET_SPEED /* TODO: per-ship */,
			_heading, _col); // FIXME - speed
}
