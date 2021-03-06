#include "timestamps.h"

Timestamps* initTimestamps(unsigned int dataSize)
{
	Timestamps *timestamps = NULL;
	timestamps = malloc(sizeof(Timestamps));

	timestamps->nextPos = 0;
	timestamps->dataSize = dataSize;
	timestamps->data = malloc(dataSize * sizeof(struct timespec));
	timestamps->pos = malloc(dataSize * sizeof(int64_t));
	timestamps->filenameSize = 200 * sizeof(char);
	timestamps->filename = malloc(timestamps->filenameSize);

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
	
	if(timestamps->filename != NULL)
	{
		free(timestamps->filename);
	}

	free(timestamps);
}

signed short int addTimestamp(Timestamps *timestamps, int64_t pos, struct timespec *timestamp)
{
	if(timestamps->nextPos < timestamps->dataSize)
	{
		timestamps->data[timestamps->nextPos].tv_sec = timestamp->tv_sec;
		timestamps->data[timestamps->nextPos].tv_nsec = timestamp->tv_nsec;
		timestamps->pos[timestamps->nextPos]  = pos;
		timestamps->nextPos++;
		return 0;
	}

	return -1;
}
