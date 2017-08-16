#ifndef DEF_TIMESTAMPS_H
#define DEF_TIMESTAMPS_H

#include <stdlib.h>
#include <time.h>

#include "utils.h"

typedef struct
{
	struct timespec *data;
	unsigned long int *pos;
	unsigned int dataSize;
	unsigned int nextPos;
} Timestamps;

Timestamps* initTimestamps(unsigned int);
void destroyTimeStamps(Timestamps*);
signed short int addTimestamp(Timestamps*, unsigned long int);

#endif
