#ifndef DEF_TIMESTAMPS_H
#define DEF_TIMESTAMPS_H

#include <stdlib.h>
#include <time.h>

#include "utils.h"

typedef struct
{
	struct timespec *data;
	unsigned int dataSize;
	unsigned int nextPos;
} Timestamps;

Timestamps* initTimestamps(unsigned int);
void destroyTimeStamps(Timestamps*);
signed short int addTimestamp(Timestamps*);

#endif
