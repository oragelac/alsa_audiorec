#include "writefilethread.h"

WriteFileThread* initWriteFileThread(RingbufferInt16 *ringbuffer, Audio *audio, Timestamps *timestamps)
{
	WriteFileThread *writefilethread = NULL;
	
	writefilethread = malloc(sizeof(WriteFileThread));
	writefilethread->ringbuffer = ringbuffer;
	writefilethread->audio = audio;
	writefilethread->timestamps = timestamps;
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
	BramsPPS *bramspps = malloc(301 * sizeof(BramsPPS));
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
	
	writefilethread->fileout = brams_open_file("test.wav", BRAMS_FILE_WRITE, &writefilethread->fileinfo, &writefilethread->fileerror);
	
	
	int16_t *data;
	data = malloc(writefilethread->ringbuffer->dataSize * sizeof(int16_t));
	
	while(writefilethread->stop != 1)
	{
		if(readData(writefilethread->ringbuffer, data, writefilethread->ringbuffer->dataSize))
		{
			brams_write_short(writefilethread->fileout, data, writefilethread->ringbuffer->dataSize);
		}
	}
	
	if(brams_write_info(writefilethread->fileout, &writefilethread->fileinfo, NULL) == -1)
	{
		fprintf(stderr, "Error : could not write file info in file\n");
	}
	
	unsigned int i;
	for(i = 0; i < writefilethread->timestamps->nextPos; ++i)
	{
		bramspps[i].position = 0;
		bramspps[i].time = convert_timespec_to_BramsTime(writefilethread->timestamps->data[i]);
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
