#include "timestamps.h"

Timestamps* initTimestamps(unsigned int dataSize)
{
	Timestamps *timestamps = NULL;
	timestamps = malloc(sizeof(Timestamps));

	timestamps->nextPos = 0;
	timestamps->dataSize = dataSize;
	timestamps->data = malloc(dataSize * sizeof(struct timespec));
	timestamps->pos = malloc(dataSize * sizeof(unsigned long int));

	return timestamps;

}
void destroyTimeStamps(Timestamps *timestamps)
{
	if(timestamps->data != NULL)
	{
		free(timestamps->data);
	}

	if(timestamps->pos != NULL)
	{
		free(timestamps->pos);
	}

	free(timestamps);
}

signed short int addTimestamp(Timestamps *timestamps, unsigned long int pos)
{
	if(timestamps->nextPos < timestamps->dataSize)
	{
		getTime(&timestamps->data[timestamps->nextPos]);
		timestamps->pos[timestamps->nextPos]  = pos;
		timestamps->nextPos++;
		return 0;
	}

	return -1;
}
