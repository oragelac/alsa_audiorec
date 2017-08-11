#include "alsa.h"

Alsa* initAlsa(unsigned int periodSize_frames)
{
	Alsa* alsa = NULL;
	alsa = malloc(sizeof(Alsa));
	alsa->buffer = NULL;
	alsa->periodSize_frames = periodSize_frames;
	alsa->sampleCounter = 0;
	return alsa;
}

void destroyAlsa(Alsa *alsa)
{
	snd_pcm_drain(alsa->handle);
	snd_pcm_close(alsa->handle);
	
	if(alsa->buffer != NULL)
	{
		free(alsa->buffer);
	}
	
	free(alsa);
}

void updateSampleCounter(Alsa *alsa)
{
	alsa->sampleCounter += alsa->periodSize_frames;
}

signed short int openPCMDevice(Alsa *alsa, Audio *audio)
{
	if(alsa == NULL)
	{
		return -1;
	}
	
	if(audio == NULL)
	{
		return -1;
	}
	
	alsa->sampleSize = audio->sampleSize;
	
	/* Open PCM device for recording (capture). */
	alsa->rc = snd_pcm_open(&(alsa->handle), audio->deviceName, SND_PCM_STREAM_CAPTURE, 0);
	
	if (alsa->rc < 0)
	{
		fprintf(stderr, "Unable to open pcm device: %s\n", snd_strerror(alsa->rc));
		return -1;
	}
	
	return 0;
}

signed short int setupPCMDevice(Alsa *alsa, Audio *audio)
{
	snd_pcm_hw_params_alloca(&(alsa->pcmParameters));

	if(snd_pcm_hw_params_any(alsa->handle, alsa->pcmParameters) < 0)
	{
		fprintf(stderr, "Cannot configure this PCM device\n");
		return -1;
	}

	if(snd_pcm_hw_params_set_access(alsa->handle, alsa->pcmParameters, SND_PCM_ACCESS_RW_INTERLEAVED) < 0)
	{
		fprintf(stderr, "Error setting access\n");
		return -1;
	}

	if(snd_pcm_hw_params_set_format(alsa->handle, alsa->pcmParameters, SND_PCM_FORMAT_S16_LE) < 0)
	{
		fprintf(stderr, "Error setting format\n");
		return -1;
	}

	if(snd_pcm_hw_params_set_channels(alsa->handle, alsa->pcmParameters, alsa->sampleSize) < 0)
	{
		fprintf(stderr, "Error setting channels\n");
		return -1;
	}

	alsa->val = doubleToUint(audio->deviceSamplerate);
	if(snd_pcm_hw_params_set_rate_near(alsa->handle, alsa->pcmParameters, &(alsa->val), &(alsa->dir)) < 0)
	{
		fprintf(stderr, "Error setting samplerate\n");
		return -1;
	}
	
	if(doubleToUint(audio->deviceSamplerate) != alsa->val)
	{
		fprintf(stderr, "The rate %uHz is not supported by your hardware. Using %uHz instead\n", doubleToUint(audio->deviceSamplerate), alsa->val);
	}

	snd_pcm_hw_params_set_period_size_near(alsa->handle, alsa->pcmParameters, &(alsa->periodSize_frames), &(alsa->dir));

	alsa->rc = snd_pcm_hw_params(alsa->handle, alsa->pcmParameters);
	
	if (alsa->rc < 0) 
	{
		fprintf(stderr, "unable to set hw parameters: %s\n", snd_strerror(alsa->rc));
		return -1;
	}
	
	snd_pcm_hw_params_get_period_time(alsa->pcmParameters, &(alsa->val), &(alsa->dir));
	alsa->periodTime = alsa->val;
	alsa->loops = 5000000 / alsa->periodTime;
	
	snd_pcm_hw_params_get_period_size(alsa->pcmParameters, &(alsa->periodSize_frames), &(alsa->dir));
	
	alsa->frameSize = alsa->sampleSize * audio->channels;
	alsa->periodSize = alsa->periodSize_frames * alsa->sampleSize * audio->channels;
	alsa->periodSize_samples = alsa->periodSize_frames * audio->channels;
	
	alsa->buffer = (char *) malloc(alsa->periodSize);
	
	return 0;
}

void record(Alsa *alsa, Data *data, Timestamps *timestamps)
{
	while (alsa->sampleCounter < 14400000)
	{
		alsa->rc = snd_pcm_readi(alsa->handle, alsa->buffer, alsa->periodSize_frames);
		updateSampleCounter(alsa); // TODO take alsa->rc in account
		
		if(alsa->sampleCounter % 48000 == 0)
		{
			addTimestamp(timestamps);
		}
		
		if (alsa->rc == -EPIPE)
		{
		  /* EPIPE means overrun */
		  fprintf(stderr, "overrun occurred\n");
		  snd_pcm_prepare(alsa->handle);
		} 
		
		else if (alsa->rc < 0) 
		{
		  fprintf(stderr, "error from read: %s\n", snd_strerror(alsa->rc));
		} 
		
		else if (alsa->rc != (int)alsa->periodSize_frames) 
		{
		  fprintf(stderr, "short read, read %d frames\n", alsa->rc);
		}
		
		unsigned int i, j;
		
		if(byteArrayToInt16ArrayLE(alsa->buffer, data->samples, alsa->periodSize) == -1)
		{
			fprintf(stderr, "error while converting bytes to int\n");
		}
		
		for(i = 0, j = 0; i < alsa->periodSize / 2; i += 2, ++j)
		{
			int16_t l = data->samples[i];
			int16_t r = data->samples[i + 1];
			data->recombinedSamples[j] = (int16_t) sqrt((l * l) + (r * r));
		}
	}
}
