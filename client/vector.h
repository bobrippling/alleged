#ifndef VECTOR_H
#define VECTOR_H

void applyvector(double *x, double *y,
		double speed, double heading);

void addvectors(double *speed, double *heading,
		double thrust, double dir);

/*
 * obj's x, y, speed & heading
 * {x,y}{min,max} are the container's stats
 * width, height are the obj's stats
 */
void clip(double *x, double *y, double *speed, double *heading,
		double xmin, double ymin, double xmax, double ymax,
		double width, double height);

inline double clampangle(double a)
{
	while(a < -M_PI)
		a += 2 * M_PI;
	while(a > M_PI)
		a -= 2 * M_PI;

	return a;
}

#ifdef ALLEGRO_H
inline fixed radtofix(double r)
{
	return ((int)(r * 128 / M_PI)) << 16;
}
#endif

#endif
