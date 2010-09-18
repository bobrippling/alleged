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
		double xlim, double ylim, double width, double height)
{
	double xcomp, ycomp;
	bool gotcomps = false;

	if(*y < 0 || *y + height > ylim){
		getcomponents(&xcomp, &ycomp, *speed, *heading);
		gotcomps = true;

		if(*y < 0)
			ycomp =  ::fabs(ycomp);
		else
			ycomp = -::fabs(ycomp);
	}

	if(*x < 0 || *x + width > xlim){
		if(!gotcomps){
			getcomponents(&xcomp, &ycomp, *speed, *heading);
			gotcomps = true;
		}

		if(*x < 0)
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
