#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "ev3_sensor.h"
#include "position.h"
#include "engines.h"
#include "sensors.h"
#include "obstacle.h"
#include "client.h"

int main(void) {

  if (ev3_init() == -1) return (1);

  printf("Waiting tacho is plugged...\n");

  initEng();

  initSensors();

 // initClient();

  explore();

  ev3_uninit();
  printf("*** ( EV3 ) Bye! ***\n");
  return 0;
}
