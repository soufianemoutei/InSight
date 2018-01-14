#include "ev3.h"

#ifndef __SENSOR__
#define __SENSOR__


typedef struct Sensors {
   uint8_t sonarSensor;
   uint8_t gyroSensor;
   uint8_t colorSensor;
} Sensors;

extern Sensors sensors;

void initSensors(); // Init the sensors
int getColorValue(); // The color value detected by the sensor
char* getColorName(int color); // The color name of the given value "colorArr"
int getSonarValue(); // The distance from an obstacle given by the sonar sensor
int getGyroValue(); // The angle given by the gyro sensor

#endif
