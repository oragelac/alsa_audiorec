#ifndef DEF_UTILS_H
#define DEF_UTILS_H

#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <stdlib.h>

#include "fcd.h"

// Conversions

unsigned int doubleToUint(double);
short signed int byteArrayToInt16ArrayLE(char*, int16_t*, size_t);

// FCD

signed short int setFCDFrequency(double);

// Time

int getTime(struct timespec*);
time_t getSeconds(struct timespec*);
long getNanoseconds(struct timespec*);
void timespecDiff(struct timespec*, struct timespec*, struct timespec*);
long long timespecToNanoseconds(struct timespec*);
double timespecToMicroseconds(struct timespec*);
double timespecToMilliseconds(struct timespec*);
double timespecToSeconds(struct timespec*);
void waitUntilNextSecond();

#endif
