#include <cstdio>
#include <allegro.h>
#include <cmath>

#include "phyobj.h"


PhyObj::PhyObj(double x, double y, double speed, double heading, double w, double h)
	: _x(x), _y(y), _speed(speed), _heading(heading), _w(w), _h(h)
{
}

PhyObj::~PhyObj()
{
}

/*
 * trig:
 *
 * zero be right
 * PI be left
 */

void PhyObj::move(double xlim, double ylim)
{
	double xcomp, ycomp;
	bool gotcomps = false;

	_x += _speed * ::cos(_heading);
	_y += _speed * ::sin(_heading);

	if(_y < 0 || _y + _h > ylim){
		getcomponents(xcomp, ycomp);
		gotcomps = true;

		if(_y < 0)
			ycomp =  ::fabs(ycomp);
		else
			ycomp = -::fabs(ycomp);
	}

	if(_x < 0 || _x + _w > xlim){
		if(!gotcomps){
			getcomponents(xcomp, ycomp);
			gotcomps = true;
		}

		if(_x < 0)
			xcomp =  ::fabs(xcomp);
		else
			xcomp = -::fabs(xcomp);
	}

	if(gotcomps)
		setcomponents(xcomp, ycomp);
}

inline void PhyObj::getcomponents(double & xcomp, double & ycomp)
{
	xcomp = _speed * ::cos(_heading);
	ycomp = _speed * ::sin(_heading);
}

inline void PhyObj::setcomponents(double xcomp, double ycomp)
{
	if(xcomp > 0)
		_heading = ::atan(ycomp / xcomp);
	else if(xcomp < 0)
		_heading = ::atan(ycomp / xcomp) + M_PI;
	else if(ycomp > 0)
		_heading =  M_PI_4;
	else
		_heading = -M_PI_4;

	_speed = ::sqrt(ycomp * ycomp + xcomp * xcomp);
}
