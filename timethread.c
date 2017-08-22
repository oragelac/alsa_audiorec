#include "timethread.h"

TimeThread* initTimeThread()
{
	TimeThread *timethread = NULL;
	
	timethread = malloc(sizeof(TimeThread));
	timethread->stop = 0;
	timethread->newSecond = 0;
	timethread->timestamp = malloc(sizeof(struct timespec));
	timethread->prev_timestamp = malloc(sizeof(struct timespec));
	
	return timethread;
}

void destroyTimeThread(TimeThread *timethread)
{
	if(timethread->timestamp != NULL)
	{
		free(timethread->timestamp);
	}
	
	if(timethread->prev_timestamp != NULL)
	{
		free(timethread->prev_timestamp);
	}
	
	free(timethread);
}

void startTimeThread(TimeThread *timethread)
{
	if(pthread_create(&timethread->thread, NULL, runTimeThread, timethread) == -1)
	{
		fprintf(stderr, "Error while starting time thread\n");
	}
}

void stopTimeThread(TimeThread *timethread)
{
	timethread->stop = 1;

	if(pthread_join(timethread->thread, NULL))
	{
		fprintf(stderr, "Error while joining filewrite thread\n");
	}
}

void* runTimeThread(void *args)
{
	TimeThread *timethread = (TimeThread*) args;
	
	clock_gettime(CLOCK_REALTIME, timethread->timestamp);
	clock_gettime(CLOCK_REALTIME, timethread->prev_timestamp);
	
	while(timethread->stop != 1)
	{
		if(timethread->timestamp->tv_sec != timethread->prev_timestamp->tv_sec)
		{
			timethread->newSecond = 1;
			clock_gettime(CLOCK_REALTIME, timethread->timestamp);
		}
		
		clock_gettime(CLOCK_REALTIME, timethread->prev_timestamp);
	}
	
	return NULL;
}
