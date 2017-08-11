#ifndef DEF_DATA_H
#define DEF_DATA_H

#include <stdint.h>
#include <stdlib.h>

typedef struct
{
	unsigned int size;
	int16_t *samples;
	int16_t *recombinedSamples;
} Data;

Data* initData(unsigned int, unsigned short int);
void destroyData(Data*);

#endif
