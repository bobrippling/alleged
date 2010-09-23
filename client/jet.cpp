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
#define BULLET_SPEED 2
#define SHOT_DELAY   100

Jet::Jet(double speed, BITMAP *bmp, struct sockaddr_in *addr)

	: PhyObj(0, 0, speed, RAND_RAD(), bmp->w, bmp->h),
		NetObj(addr),
		_bmp(bmp),
		_col(makecol(0, 255, 0)),
		_thrust(false),
		_firing(false),
		_facing(RAND_RAD()),
		_lastshot(time_now())
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
		_firing(false),
		_facing(facing),
		_lastshot(time_now())
{
}

Jet::~Jet()
{
	destroy_bitmap(_bmp);
}

void Jet::draw(::BITMAP *buffer) const
{
	::rotate_sprite(buffer, _bmp, _x, _y, radtofix(_facing));
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
	if(_firing){
		long now = ::time_now();

		if(now - _lastshot > SHOT_DELAY){
			_lastshot = now;
			return true;
		}
	}
	return false;
}

Bullet *Jet::createbullet()
{
	double speed = _speed, heading = _heading;

	addvectors(&speed, &heading, BULLET_SPEED, _facing);

	return new Bullet(_x + _w/2, _y + _h/2, speed, heading, _col);
}
