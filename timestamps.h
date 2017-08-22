#ifndef DEF_TIMESTAMPS_H
#define DEF_TIMESTAMPS_H

#include <stdlib.h>
#include <time.h>

#include "utils.h"

typedef struct
{
	struct timespec *data;
	unsigned long *pos;
	unsigned int dataSize;
	unsigned int nextPos;
	char* filename;
	unsigned int filenameSize;
} Timestamps;

Timestamps* initTimestamps(unsigned int);
void destroyTimeStamps(Timestamps*);
signed short int addTimestamp(Timestamps*, unsigned long);
signed short int saveTimestamp(Timestamps*, unsigned long, struct timespec*);

#endif
