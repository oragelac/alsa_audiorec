#ifndef DEF_AUDIODEVICE_H
#define DEF_AUDIODEVICE_H

#include <string.h>
#include <stdlib.h>

typedef struct
{
	char* deviceName;
	double deviceFrequency;
	double deviceSamplerate;
	double waveSamplerate;
	double resampleRatio;
	unsigned int channels;
	unsigned short int sampleSize;
} Audio;

Audio* initAudio(char*, double, double, double, unsigned int, unsigned short int);
void destroyAudio(Audio*);

#endif
