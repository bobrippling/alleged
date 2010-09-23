#include <cstdio>
#include <allegro.h>
#include <cmath>

#include "phyobj.h"

#include "vector.h"


PhyObj::PhyObj(double x, double y, double speed, double heading, double w, double h)
	: _x(x), _y(y), _speed(speed), _heading(heading), _w(w), _h(h)
{
}

PhyObj::~PhyObj()
{
}

void PhyObj::move(double xlim, double ylim)
{
	::applyvector(&_x, &_y, _speed, _heading);

	::clip(&_x, &_y, &_speed, &_heading,
			_w, _h, xlim, ylim);
}
