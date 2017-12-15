#include "ev3.h"

#ifndef __SENSOR__
#define __SENSOR__


typedef struct Sensors {
   uint8_t sonarSensor;
   uint8_t gyroSensor;
   uint8_t colorSensor;
   uint8_t compassSensor;
} Sensors;

extern Sensors sensors;

void initSensors();
int getColorValue();
int getSonarValue();
float getGyroValue();
float getCompassValue();

#endif
