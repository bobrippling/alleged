#include <cmath>

#include "vector.h"

static inline void getcomponents(double *xc, double *yc, double  speed, double  heading);
static inline void setcomponents(double  xc, double  yc, double *speed, double *heading);

/*
 * trig:
 *
 * zero be right
 * M_PI be left
 */

// TODO: prefix all vars with 'register'

void applyvector(double *x, double *y, double speed, double heading)
{
	*x += speed * ::cos(heading);
	*y += speed * ::sin(heading);
}

void addvectors(double *speed, double *heading, double thrust, double dir)
{
	double xc1, yc1, xc2, yc2;

	getcomponents(&xc1, &yc1, *speed, *heading);
	getcomponents(&xc2, &yc2, thrust, dir);

	setcomponents(xc1 + xc2, yc1 + yc2, speed, heading);
}

void clip(double *x, double *y, double *speed, double *heading,
		double width, double height,
		double xmax, double ymax,
		// optional below
		double xmin, double ymin)
{
	double xcomp, ycomp;
	bool gotcomps = false;

	if(*y < ymin || *y + height > ymax){
		getcomponents(&xcomp, &ycomp, *speed, *heading);
		gotcomps = true;

		if(*y < ymin)
			ycomp =  ::fabs(ycomp);
		else
			ycomp = -::fabs(ycomp);
	}

	if(*x < xmin || *x + width > xmax){
		if(!gotcomps){
			getcomponents(&xcomp, &ycomp, *speed, *heading);
			gotcomps = true;
		}

		if(*x < xmin)
			xcomp =  ::fabs(xcomp);
		else
			xcomp = -::fabs(xcomp);
	}

	if(gotcomps)
		setcomponents(xcomp, ycomp, speed, heading);
}

static inline void getcomponents(double *xcomp, double *ycomp, double speed, double heading)
{
	*xcomp = speed * ::cos(heading);
	*ycomp = speed * ::sin(heading);
}

static inline void setcomponents(double xcomp, double ycomp, double *speed, double *heading)
{
	if(xcomp > 0)
		*heading = ::atan(ycomp / xcomp);
	else if(xcomp < 0)
		*heading = ::atan(ycomp / xcomp) + M_PI;
	else if(ycomp > 0)
		*heading =  M_PI_4;
	else
		*heading = -M_PI_4;

	*speed = ::sqrt(ycomp * ycomp + xcomp * xcomp);
}
