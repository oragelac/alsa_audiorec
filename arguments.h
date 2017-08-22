#ifndef DEF_ARGUMENTS_H
#define DEF_ARGUMENTS_H

#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef struct
{
	int argc;
	char **argv;
	char *optString;
	
	char *deviceName;
	double frequency;
	unsigned int samplerate;
	double waveSamplerate;
	unsigned short int channels;
	unsigned short int sampleSize;
	unsigned int ringbufferSize;
	unsigned int duration;
	unsigned short int periodSize;	
} Arguments;

Arguments* initArguments(int, char**, char*);
void destroyArguments(Arguments*);
void parseArgs(Arguments*);

#endif
