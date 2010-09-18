#ifndef VECTOR_H
#define VECTOR_H

void applyvector(double *x, double *y,
		double speed, double heading);

void addvectors(double *speed, double *heading,
		double thrust, double dir);

void clip(double *x, double *y, double *speed, double *heading,
		double xlim, double ylim, double width, double height);

// FIXME
inline double clipangle(double a)
{
	while(a < -M_PI)
		a += M_2_PI;
	while(a > M_PI)
		a -= M_2_PI;
	return a;
}

#endif
