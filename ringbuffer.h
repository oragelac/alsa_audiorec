#ifndef DEF_RINGBUFFERINT16_H
#define DEF_RINGBUFFERINT16_H

#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

typedef struct
{
	int16_t **buffer;
	unsigned int bufferSize;
	unsigned int dataSize;
	unsigned int writePtr;
	unsigned int readPtr;
	unsigned int freeSpace;
	unsigned int sleepTime_us;

} RingbufferInt16;

RingbufferInt16* initRingbufferInt16(unsigned int, unsigned int, unsigned int);
void destroyRingbufferInt16(RingbufferInt16*);
unsigned int writeData(RingbufferInt16*, int16_t*, unsigned int);
unsigned int readData(RingbufferInt16*, int16_t*, unsigned int);

#endif
