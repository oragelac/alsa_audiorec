#include "utils.h"


//Conversions

unsigned int doubleToUint(double d)
{
	d += 0.5;
	return (unsigned int) d;
}

short signed int byteArrayToInt16ArrayLE(char *src, size_t srcSize, int16_t *dst)
{
	unsigned int i;

	if(src == NULL || dst == NULL)
	{
		return -1;
	}

	for(i = 0; i < srcSize; i += 2)
	{
		dst[i / 2] = src[i] | (int16_t) src[i + 1] << 8;
	}

	return 0;
}

int16_t* byteArrayToNewInt16ArrayLE(char *src, size_t srcSize)
{
	unsigned int i;
	int16_t *dst = NULL;

	dst = malloc((srcSize / 2) * sizeof(int16_t));

	for(i = 0; i < srcSize; i += 2)
	{
		dst[i / 2] = src[i] | (int16_t) src[i + 1] << 8;
	}

	return dst;
}

// FCD

signed short int setFCDFrequency(double frequency)
{
	int stat;
	int corr = 0;
  double freqf = frequency;
	int freq = (int)(freqf * 1.0e6f);

	/* calculate frequency */
	freq *= 1.0 + corr / 1000000.0;

	/* set it */
	stat = fcdAppSetFreq(freq);

	if (stat == FCD_MODE_NONE)
	{
		printf("No FCD Detected.\n");
		return -1;
	}
	else if (stat == FCD_MODE_BL)
	{
		printf("FCD in bootloader mode.\n");
		return -1;
	}

	printf("Freq set to %.6f MHz.\n", freq/1e6);
	return 0;
}

// Time

inline int getTime(struct timespec *ts)
{
	return clock_gettime(CLOCK_REALTIME, ts);
}

time_t getSeconds(struct timespec *ts)
{
	return ts->tv_sec;
}

long getNanoseconds(struct timespec *ts)
{
	return ts->tv_nsec;
}

void timespecDiff(struct timespec *start, struct timespec *stop, struct timespec *result)
{
	if((stop->tv_nsec - start->tv_nsec) < 0)
	{
		result->tv_sec = stop ->tv_sec - start->tv_sec - 1;
		result->tv_nsec = stop->tv_nsec - start->tv_nsec + 1000000000;
	}

	else
	{
		result->tv_sec = stop->tv_sec - start->tv_sec;
		result->tv_nsec = stop->tv_nsec - start->tv_nsec;
	}
}

inline int64_t timespecToNanoseconds(struct timespec *ts)
{
	int64_t ns;

	ns = (int64_t) ts->tv_sec * 1000000000 + (int64_t) ts->tv_nsec;
	return ns;
}

double timespecToMicroseconds(struct timespec *ts)
{
	double us;

	us = (double) timespecToNanoseconds(ts) / (double) 1000;
	return us;
}

double timespecToMilliseconds(struct timespec *ts)
{
	double ms;

	ms = (double) timespecToNanoseconds(ts) / (double) 1000000;
	return ms;
}

double timespecToSeconds(struct timespec *ts)
{
	double s;

	s = (double) timespecToNanoseconds(ts) / (double) 1000000000;
	return s;
}

inline void waitUntilNextSecond()
{
	struct timespec *currentTime;
	struct timespec *previousTime;

	currentTime = malloc(sizeof(struct timespec));
	previousTime = malloc(sizeof(struct timespec));

	getTime(previousTime);

	do
	{
		getTime(currentTime);
	} while(getSeconds(currentTime) == getSeconds(previousTime));

	free(currentTime);
	free(previousTime);
}

BramsTime convert_timespec_to_BramsTime(struct timespec ts)
{
	return ((BramsTime)ts.tv_sec * (BramsTime)1000000) + ((BramsTime)ts.tv_nsec / (BramsTime)1000);		
}
