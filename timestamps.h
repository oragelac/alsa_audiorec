#ifndef DEF_TIMESTAMPS_H
#define DEF_TIMESTAMPS_H

#include <stdlib.h>
#include <time.h>

#include "utils.h"

typedef struct
{
	struct timespec *data;
	int64_t *pos;
	unsigned int dataSize;
	unsigned int nextPos;
	char* filename;
	unsigned int filenameSize;
} Timestamps;

Timestamps* initTimestamps(unsigned int);
void destroyTimeStamps(Timestamps*);
signed short int addTimestamp(Timestamps*, int64_t, struct timespec*);

#endif
