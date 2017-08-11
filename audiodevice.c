#include "audiodevice.h"

Audio* initAudio(char *deviceName, double deviceFrequency, double deviceSamplerate, double waveSamplerate, unsigned int channels, unsigned short int sampleSize)
{
	Audio *parameters;
	
	parameters = malloc(sizeof(Audio));
	
	parameters->deviceName = malloc(strlen(deviceName) + 1);
	strcpy(parameters->deviceName, deviceName);
	parameters->deviceFrequency = deviceFrequency;
	parameters->deviceSamplerate = deviceSamplerate;
	parameters->waveSamplerate = waveSamplerate;
	parameters->resampleRatio = parameters->waveSamplerate / parameters->deviceSamplerate;
	parameters->channels = channels;
	parameters->sampleSize = sampleSize;
	
	return parameters;
}

void destroyAudio(Audio *parameters)
{
	if(parameters->deviceName != NULL)
	{
		free(parameters->deviceName);
	}
	
	free(parameters);
}
