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
#include "timethread.h"

int main(int argc, char **argv) 
{
	Audio *audio = NULL;
	Alsa *alsa = NULL;
	Arguments *args = NULL;
	Data *data = NULL;
	Timestamps *timestamps = NULL;
	WriteFileThread *writefilethread = NULL;
	RingbufferInt16 *ringbuffer = NULL;
	TimeThread *timethread = NULL;
	
	args = initArguments(argc, argv, "d:f:r:w:c:s:b:t:p:");
	parseArgs(args);
	audio = initAudio(args->deviceName, args->frequency, args->samplerate, args->waveSamplerate, args->channels, args->sampleSize);
	alsa = initAlsa(args->periodSize, args->duration);
	timestamps = initTimestamps(args->duration + 1);
	timethread = initTimeThread();
	
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
	ringbuffer = initRingbufferInt16(args->ringbufferSize, data->recombinedDataSize);
	writefilethread = initWriteFileThread(ringbuffer, audio, timestamps, alsa);
	startWriteFileThread(writefilethread);
	waitUntilNextSecond();
	startTimeThread(timethread);
	record(alsa, audio, data, timestamps, ringbuffer, timethread);
	
	printf("%d saved timestamps\n", timestamps->nextPos);
	
	stopTimeThread(timethread);
	stopWriteFileThread(writefilethread);
	
	destroyTimeThread(timethread);
	destroyWriteFileThread(writefilethread);
	destroyTimeStamps(timestamps);
	destroyData(data);
	destroyAlsa(alsa);
	destroyAudio(audio);
	destroyArguments(args);
	destroyRingbufferInt16(ringbuffer);
	
	return EXIT_SUCCESS;
}
