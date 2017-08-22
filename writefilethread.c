#include "writefilethread.h"

WriteFileThread* initWriteFileThread(RingbufferInt16 *ringbuffer, Audio *audio, Timestamps *timestamps, Alsa *alsa)
{
	WriteFileThread *writefilethread = NULL;

	writefilethread = malloc(sizeof(WriteFileThread));
	writefilethread->ringbuffer = ringbuffer;
	writefilethread->audio = audio;
	writefilethread->timestamps = timestamps;
	writefilethread->alsa = alsa;
	writefilethread->stop = 0;
	writefilethread->fileout = NULL;
	writefilethread->fileerror = NULL;

	return writefilethread;
}

void destroyWriteFileThread(WriteFileThread *writefilethread)
{
	free(writefilethread);
}

void startWriteFileThread(WriteFileThread *writefilethread)
{
	if(pthread_create(&writefilethread->thread, NULL, run, writefilethread) == -1)
	{
		fprintf(stderr, "Error while starting filewrite thread\n");
	}
}

void* run(void *args)
{
	BramsPPS *bramspps = malloc(300 * sizeof(BramsPPS));
	WriteFileThread *writefilethread = (WriteFileThread*) args;

	memset(&writefilethread->fileinfo, 0, sizeof(BramsFileInfo));
	writefilethread->fileinfo.version = 4;
	writefilethread->fileinfo.sample_rate = writefilethread->audio->waveSamplerate;
	writefilethread->fileinfo.n_channels = 1;
	writefilethread->fileinfo.lo_frequency = 49969000.0;
	writefilethread->fileinfo.antenna_id = 1;
	writefilethread->fileinfo.beacon_latitude = 50.0972;
	writefilethread->fileinfo.beacon_longitude = 4.5847;
	writefilethread->fileinfo.beacon_altitude = 231.0;
	writefilethread->fileinfo.beacon_frequency = 49970000.0;
	writefilethread->fileinfo.beacon_power = 150.0;
	writefilethread->fileinfo.beacon_polarisation = BRAMS_RHCP;
	strcpy(writefilethread->fileinfo.beacon_code, "BEDOUR");
	strcpy(writefilethread->fileinfo.station_code, "TEST00");


	short unsigned int isFileOpened = 0;
	int16_t *data;
	char filename[200];
	const char* fmt = "RAD_BEDOUR_%Y%m%d_%H%M_TEST00_SYS001.wav";
	data = malloc(writefilethread->ringbuffer->dataSize * sizeof(int16_t));
	unsigned int i;

	while(writefilethread->stop != 1)
	{
		if(readData(writefilethread->ringbuffer, data, writefilethread->ringbuffer->dataSize))
		{
			if(isFileOpened == 0)
			{
				struct tm *tmp;
				struct timespec ts;
				getTime(&ts);
				tmp = gmtime(&ts.tv_sec);
				strftime(filename, sizeof(filename), fmt, tmp);
				writefilethread->fileout = brams_open_file(filename, BRAMS_FILE_WRITE, &writefilethread->fileinfo, &writefilethread->fileerror);
				isFileOpened = 1;
			}
			
			brams_write_short(writefilethread->fileout, data, writefilethread->ringbuffer->dataSize);
		}
	}

	if(brams_write_info(writefilethread->fileout, &writefilethread->fileinfo, NULL) == -1)
	{
		fprintf(stderr, "Error : could not write file info in file\n");
	}
	
	for(i = 0; i < writefilethread->timestamps->nextPos; ++i)
	{
		bramspps[i].position = writefilethread->timestamps->pos[i];
		double g = (double)writefilethread->timestamps->data[i].tv_nsec * 1e-9;
		double h = (lround(g) - g) * writefilethread->audio->waveSamplerate;
		writefilethread->timestamps->data[i].tv_nsec = 0;
		writefilethread->timestamps->data[i].tv_sec += (lround(g) - g);
		bramspps[i].time = timespecToBramsTime(&writefilethread->timestamps->data[i]);
		bramspps[i].position = (BramsCount) (bramspps[i].position + h);					
	}	

	if(brams_write_pps(writefilethread->fileout, bramspps, writefilethread->timestamps->nextPos) == -1)
	{
		fprintf(stderr, "Error : could not save PPS in file\n");
	}

	brams_close_file(writefilethread->fileout);
	writefilethread->fileout = NULL;
	free(data);
	free(bramspps);
	return NULL;
}

void stopWriteFileThread(WriteFileThread *writefilethread)
{
	writefilethread->stop = 1;

	if(pthread_join(writefilethread->thread, NULL))
	{
		fprintf(stderr, "Error while joining filewrite thread\n");
	}
}
