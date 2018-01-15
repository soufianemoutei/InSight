#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "ev3_sensor.h"
#include "engines.h"
#include "sensors.h"
#include "client.h"
#include "main.h"

FILE* printFile = NULL;

int main(void) {

  if (ev3_init() == -1) return (1);

  printFile = fopen(PRINT_FILE, "w+");

  initEng();

  initSensors();

  initClient();

  ev3_uninit();

  fprintf(printFile,"END\n");

  if (printFile != NULL) {
    close(printFile);
  }

  return 0;
}

int getNearestInteger(float f) {
	return ((int) round(f));
}
