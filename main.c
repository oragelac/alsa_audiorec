/* Use the newer ALSA API */
#define ALSA_PCM_NEW_HW_PARAMS_API

#include <stdlib.h>
#include <stdint.h>
#include <alsa/asoundlib.h>
#include "alsa.h"
#include "audiodevice.h"
#include "utils.h"
#include "arguments.h"
#include "data.h"
#include "timestamps.h"
#include "writefilethread.h"
#include "ringbuffer.h"

int main(int argc, char **argv) 
{
	Audio *audio = NULL;
	Alsa *alsa = NULL;
	Arguments *args = NULL;
	Data *data = NULL;
	Timestamps *timestamps = NULL;
	WriteFileThread *writefilethread = NULL;
	RingbufferInt16 *ringbuffer = NULL;
	
	args = initArguments(argc, argv, "d:f:r:w:c:s:b:");
	parseArgs(args);
	audio = initAudio(args->deviceName, args->frequency, args->samplerate, args->waveSamplerate, args->channels, args->sampleSize);
	alsa = initAlsa(48);
	timestamps = initTimestamps(301);
	
	if(setFCDFrequency(audio->deviceFrequency) < 0)
	{
		return EXIT_FAILURE;
	}
	
	if(openPCMDevice(alsa, audio) < 0)
	{
		return EXIT_FAILURE;
	}
	
	if(setupPCMDevice(alsa, audio) < 0)
	{
		return EXIT_FAILURE;
	}
	
	data = initData(alsa->periodSize / alsa->sampleSize, audio->channels);
	ringbuffer = initRingbufferInt16(args->ringbufferSize, data->recombinedDataSize, 500);
	writefilethread = initWriteFileThread(ringbuffer, audio, timestamps);
	startWriteFileThread(writefilethread);
	waitUntilNextSecond();
	record(alsa, audio, data, timestamps, ringbuffer);
	
	printf("%d saved timestamps\n", timestamps->nextPos);
	
	
	unsigned int i;
	struct timespec diff;
	
	for(i = 0; i < timestamps->nextPos; ++i)
	{
		if(i > 0)
		{
			timespecDiff(&timestamps->data[i - 1], &timestamps->data[i], &diff);
			printf("%lf\t\t%lf\n", timespecToSeconds(&timestamps->data[i]), timespecToSeconds(&diff));
		}
		else
		{
			printf("%lf\n", timespecToSeconds(&timestamps->data[i]));
		}
	}
	
	
	stopWriteFileThread(writefilethread);
	
	destroyWriteFileThread(writefilethread);
	destroyTimeStamps(timestamps);
	destroyData(data);
	destroyAlsa(alsa);
	destroyAudio(audio);
	destroyArguments(args);
	destroyRingbufferInt16(ringbuffer);
	
	return EXIT_SUCCESS;
}
