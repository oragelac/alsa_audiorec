#ifndef DEF_ALSA_H
#define DEF_ALSA_H

#include <alsa/asoundlib.h>
#include <math.h>
#include <stdlib.h>

#include "utils.h"
#include "audiodevice.h"
#include "data.h"
#include "utils.h"
#include "timestamps.h"

typedef struct
{
	char *buffer;
	snd_pcm_hw_params_t *pcmParameters;
	snd_pcm_t *handle;
	int rc;
	unsigned int val;
	int dir;
	snd_pcm_uframes_t periodSize_frames;
	unsigned int periodSize_samples;
	unsigned int periodSize;
	unsigned int frameSize;
	unsigned int sampleSize;
	unsigned int periodTime;
	unsigned long loops;
	unsigned long sampleCounter;
} Alsa;

Alsa* initAlsa(unsigned int);
void destroyAlsa(Alsa*);
void updateSampleCounter(Alsa*);
signed short int openPCMDevice(Alsa*, Audio*);
signed short int setupPCMDevice(Alsa*, Audio*);
void record(Alsa*, Data*, Timestamps*);

#endif
