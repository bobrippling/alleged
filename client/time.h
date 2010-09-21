#ifndef TIME_H
#define TIME_H

#include <sys/time.h>

inline int time_now()
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return ((unsigned long)tv.tv_sec) * 1000000 + tv.tv_usec;
}

#endif
