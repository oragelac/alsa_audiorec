#ifndef DEF_TIMETHREAD_H
#define DEF_TIMETHREAD_H

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "utils.h"

typedef struct
{
	pthread_t thread;
	short unsigned int stop;
	struct timespec *timestamp;
	struct timespec *prev_timestamp;
	unsigned short int newSecond;
} TimeThread;

TimeThread* initTimeThread();
void destroyTimeThread(TimeThread*);
void startTimeThread(TimeThread*);
void stopTimeThread(TimeThread*);
void* runTimeThread(void*);

#endif

