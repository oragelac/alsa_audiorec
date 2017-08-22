#ifndef DEF_WRITEFILETHREAD_H
#define DEF_WRITEFILETHREAD_H

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <brams.h>

#include "ringbuffer.h"
#include "audiodevice.h"
#include "timestamps.h"
#include "utils.h"
#include "alsa.h"

typedef struct
{
	pthread_t thread;
	short unsigned int stop;
	RingbufferInt16 *ringbuffer;
	Audio *audio;
	Timestamps *timestamps;
	Alsa *alsa;
	BramsFile *fileout;
	BramsFileInfo fileinfo;
	BramsError *fileerror;
} WriteFileThread;

WriteFileThread* initWriteFileThread(RingbufferInt16*, Audio*, Timestamps*, Alsa*);
void destroyWriteFileThread(WriteFileThread*);
void startWriteFileThread(WriteFileThread*);
void stopWriteFileThread(WriteFileThread*);
void* run(void*);

#endif
