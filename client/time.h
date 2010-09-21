#ifndef TIME_H
#define TIME_H

#include <sys/time.h>

inline int time_now()
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

#endif
