#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include "sensors.h"
#include "ev3.h"
#include "ev3_sensor.h"
#include "ev3_port.h"
#include "engines.h"

Sensors sensors;

void initSensors() {

  printf("Sensor initialization...\n");

  while (ev3_sensor_init() < 1) sleep(1);

  if (!ev3_search_sensor(LEGO_EV3_US,&(sensors.sonarSensor),0)) {
    sensors.sonarSensor = DESC_LIMIT;
    printf("WARNING: Sonar sensor was NOT found!\n");
  } else {
    set_sensor_mode(sensors.sonarSensor,"US-DIST-CM");
    printf("Sonar sensor was found!\n");
  }

  if (!ev3_search_sensor(LEGO_EV3_GYRO,&(sensors.gyroSensor),0)) {
    sensors.gyroSensor = DESC_LIMIT;
    printf("WARNING: Gyro sensor was NOT found!\n");
  } else {
    printf("Gyro sensor was found!\n");
    set_sensor_mode(sensors.gyroSensor, "GYRO-CAL");
    Sleep(200);
    set_sensor_mode(sensors.gyroSensor, "GYRO-ANG");
    Sleep(200);
  }

  if (!ev3_search_sensor(LEGO_EV3_COLOR, &(sensors.colorSensor),0)) {
    sensors.colorSensor = DESC_LIMIT;
    printf("WARNING: Color sensor was NOT found!\n");
  } else {
    printf("Color sensor was found!\n");
    set_sensor_mode(sensors.colorSensor,"COL-COLOR");
    Sleep(200);
  }

  if (!ev3_search_sensor(HT_NXT_COMPASS, &(sensors.compassSensor),0)) {
    sensors.compassSensor = DESC_LIMIT;
    printf("WARNING: Compass sensor was NOT found!\n");
  } else {
    printf("Compass sensor was found!\n");
    set_sensor_mode(sensors.compassSensor,"COMPASS");
    Sleep(200);
  }
}

int getSonarValue() {
  int val;
  get_sensor_value(0, sensors.sonarSensor, &val);
  return val;
}

int getColorValue() {
  int val;
  get_sensor_value(0, sensors.colorSensor, &val);
  return val;
}

float getGyroValue() {
  float angle;
  get_sensor_value0(sensors.gyroSensor,&angle);
  return angle;
}

float getCompassValue() {
  float angle;
  get_sensor_value0(sensors.compassSensor,&angle);
  return angle;
}
