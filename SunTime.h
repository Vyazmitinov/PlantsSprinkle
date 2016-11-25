#ifndef PUMPER_SUN_TIME_H
#define PUMPER_SUN_TIME_H

#include <stdint.h>

float getSunTime(uint8_t day, uint8_t month, int year, bool sunrise, int timezone, float longitude, float latitude);

uint8_t getMinutes(float time);

#endif // PUMPER_SUN_TIME_H

