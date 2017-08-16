#include "ringbuffer.h"


RingbufferInt16* initRingbufferInt16(unsigned int bufferSize, unsigned int dataSize, unsigned int sleepTime_us)
{
	RingbufferInt16 *ringbuffer = NULL;
	
	ringbuffer = malloc(sizeof(RingbufferInt16));
	ringbuffer->bufferSize = bufferSize;
	ringbuffer->dataSize = dataSize;
	ringbuffer->sleepTime_us = sleepTime_us;
	ringbuffer->writePtr = 0;
	ringbuffer->readPtr = 0;
	ringbuffer->freeSpace = ringbuffer->bufferSize;
	ringbuffer->buffer = malloc(ringbuffer->bufferSize * sizeof(int16_t*));
	
	unsigned int i;
		
	for(i = 0; i < ringbuffer->bufferSize; ++i)
	{
		ringbuffer->buffer[i] = malloc(ringbuffer->dataSize * sizeof(int16_t));
	}
	
	return ringbuffer;
}

void destroyRingbufferInt16(RingbufferInt16 *ringbuffer)
{
	if(ringbuffer->buffer != NULL)
	{
		unsigned int i;
		
		for(i = 0; i < ringbuffer->bufferSize; ++i)
		{
			free(ringbuffer->buffer[i]);
		}
		
		free(ringbuffer->buffer);
	}
	
	free(ringbuffer);
}

unsigned int writeData(RingbufferInt16 *ringbuffer, int16_t *data, unsigned int dataSize)
{
	if(ringbuffer->freeSpace == 0 || dataSize > ringbuffer->dataSize)
	{
		printf("No more space in ringbuffer\n");
		//usleep(ringbuffer->sleepTime_us);
		return 0;
	}
	
	unsigned int i;
	
	for(i = 0; i < dataSize; ++i)
	{
		ringbuffer->buffer[ringbuffer->writePtr][i] = data[i];
	}
	
	ringbuffer->writePtr = (ringbuffer->writePtr + 1) % ringbuffer->bufferSize;
	ringbuffer->freeSpace--;
	return dataSize;
}

unsigned int readData(RingbufferInt16 *ringbuffer, int16_t *data, unsigned int dataSize)
{
	if(ringbuffer->readPtr == ringbuffer->writePtr || dataSize > ringbuffer->dataSize)
	{
		//printf("No more data to read\n");
		//usleep(ringbuffer->sleepTime_us);
		return 0;
	}
	
	unsigned int i;
	
	for(i = 0; i < dataSize; ++i)
	{
		data[i] = ringbuffer->buffer[ringbuffer->readPtr][i];
	}
	
	ringbuffer->readPtr = (ringbuffer->readPtr + 1) % ringbuffer->bufferSize;
	ringbuffer->freeSpace++;
	
	return dataSize;
}
