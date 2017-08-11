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

int main(int argc, char **argv) 
{
	Audio *audio = NULL;
	Alsa *alsa = NULL;
	Arguments *args = NULL;
	Data *data = NULL;
	Timestamps *timestamps = NULL;
	
	args = initArguments(argc, argv, "d:f:r:w:c:s:");
	parseArgs(args);
	audio = initAudio(args->deviceName, args->frequency, args->samplerate, args->waveSamplerate, args->channels, args->sampleSize);
	alsa = initAlsa(48);
	timestamps = initTimestamps(350);
	
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
	waitUntilNextSecond();
	record(alsa, data, timestamps);
	
	printf("%d saved timestamps\n", timestamps->nextPos);
	
	unsigned int i;
	
	FILE* f;
	
	f = fopen("timestamps.csv", "w");
	fprintf(f, "time(s)\t\t\tdiff(s)\n");
	
	struct timespec diff;
	
	for(i = 0; i < timestamps->nextPos; ++i)
	{
		if(i > 0)
		{
			timespecDiff(&timestamps->data[i - 1], &timestamps->data[i], &diff);
			fprintf(f, "%lf\t\t%lf\n", timespecToSeconds(&timestamps->data[i]), timespecToSeconds(&diff));
		}
		else
		{
			fprintf(f, "%lf\n", timespecToSeconds(&timestamps->data[i]));
		}
	}
	
	fclose(f);
	
	destroyTimeStamps(timestamps);
	destroyData(data);
	destroyAlsa(alsa);
	destroyAudio(audio);
	destroyArguments(args);
	
	return EXIT_SUCCESS;
}
