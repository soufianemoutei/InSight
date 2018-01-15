#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include "ev3.h"
#include "ev3_sensor.h"
#include "ev3_port.h"
#include "main.h"
#include "sensors.h"

Sensors sensors;

void initSensors() {

  fprintf(printFile,"Sensor initialization...\n");

  while (ev3_sensor_init() < 1) sleep(1);

  if (!ev3_search_sensor(LEGO_EV3_US,&(sensors.sonarSensor),0)) {
    sensors.sonarSensor = DESC_LIMIT;
    fprintf(printFile,"WARNING: Sonar sensor was NOT found!\n");
  } else {
    set_sensor_mode(sensors.sonarSensor,"US-DIST-CM");
    fprintf(printFile,"Sonar sensor was found!\n");
  }

  if (!ev3_search_sensor(LEGO_EV3_GYRO,&(sensors.gyroSensor),0)) {
    sensors.gyroSensor = DESC_LIMIT;
    fprintf(printFile,"WARNING: Gyro sensor was NOT found!\n");
  } else {
    fprintf(printFile,"Gyro sensor was found!\n");
    set_sensor_mode(sensors.gyroSensor, "GYRO-CAL");
    Sleep(200);
    set_sensor_mode(sensors.gyroSensor, "GYRO-ANG");
    Sleep(200);
  }

  if (!ev3_search_sensor(LEGO_EV3_COLOR, &(sensors.colorSensor),0)) {
    sensors.colorSensor = DESC_LIMIT;
    fprintf(printFile,"WARNING: Color sensor was NOT found!\n");
  } else {
    fprintf(printFile,"Color sensor was found!\n");
    set_sensor_mode(sensors.colorSensor,"COL-COLOR");
    Sleep(200);
  }

}

int getSonarValue() {
  int val;
  if (sensors.sonarSensor == DESC_LIMIT) {
    fprintf(printFile,"CANNOT USE SONAR SENSOR\n");
    return 0;
  }
  get_sensor_value(0, sensors.sonarSensor, &val);
  return val;
}

int getColorValue() {
  int val;
  if (sensors.colorSensor == DESC_LIMIT) {
    fprintf(printFile,"CANNOT USE COLOR SENSOR\n");
    return 0;
  }
  get_sensor_value(0, sensors.colorSensor, &val);
  return val;
}

char* getColorName(int color) {
  char* colorArr[] = { "?", "BLACK", "BLUE", "GREEN", "YELLOW", "RED", "WHITE", "BROWN" };
  return colorArr[color];
}

int getGyroValue() {
  int angle;
  if (sensors.gyroSensor == DESC_LIMIT) {
    fprintf(printFile,"CANNOT USE GYRO SENSOR\n");
    return 0;
  }
  get_sensor_value(0, sensors.gyroSensor,&angle);
  return angle;
}
