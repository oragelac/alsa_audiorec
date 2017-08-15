#ifndef DEF_UTILS_H
#define DEF_UTILS_H

#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <stdlib.h>

#include "fcd.h"

// Conversions

unsigned int doubleToUint(double);
short signed int byteArrayToInt16ArrayLE(char*, size_t, int16_t*);
int16_t* byteArrayToInt16ArrayLE(char*, size_t);

// FCD

signed short int setFCDFrequency(double);

// Time

inline int getTime(struct timespec*);
time_t getSeconds(struct timespec*);
long getNanoseconds(struct timespec*);
void timespecDiff(struct timespec*, struct timespec*, struct timespec*);
long long timespecToNanoseconds(struct timespec*);
double timespecToMicroseconds(struct timespec*);
double timespecToMilliseconds(struct timespec*);
double timespecToSeconds(struct timespec*);
inline void waitUntilNextSecond();

#endif
