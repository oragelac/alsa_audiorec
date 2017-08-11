#include "data.h"

Data* initData(unsigned int size, unsigned short int channels)
{
	Data *data = NULL;
	
	data = malloc(sizeof(Data));
	
	data->size = size;
	data->samples = malloc(data->size * sizeof(int16_t));
	data->recombinedSamples = malloc(data->size * sizeof(int16_t) / channels);
	
	return data;
}
void destroyData(Data *data)
{
	if(data->samples != NULL)
	{
		free(data->samples);
	}
	
	if(data->recombinedSamples != NULL)
	{
		free(data->recombinedSamples);
	}
}
