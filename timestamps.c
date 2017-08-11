#include "timestamps.h"

Timestamps* initTimestamps(unsigned int dataSize)
{
	Timestamps *timestamps = NULL;
	timestamps = malloc(sizeof(Timestamps));
	
	timestamps->nextPos = 0;
	timestamps->dataSize = dataSize;
	timestamps->data = malloc(dataSize * sizeof(struct timespec));
	
	return timestamps;
	
}
void destroyTimeStamps(Timestamps *timestamps)
{
	if(timestamps->data != NULL)
	{
		free(timestamps->data);
	}
	
	free(timestamps);
}

signed short int addTimestamp(Timestamps *timestamps)
{
	if(timestamps->nextPos < timestamps->dataSize)
	{
		getTime(&timestamps->data[timestamps->nextPos]);
		timestamps->nextPos++;
		return 0;
	}
	
	return -1;
}
