#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_sensor.h"
#include "ev3_tacho.h"
#include "engines.h"
#include "sensors.h"
#include "obstacle.h"
#include "position.h"
#include <time.h>

int length;
int weidth;

int callsOfdetectObstacles = 0;

void detectObstacles() {
  clock_t before, difference;
  int sec;
  callsOfdetectObstacles++;
  turn(((callsOfdetectObstacles == 1) ? 1 : -1) * 90);
  sleep(2);
  goStraight(0);
  before = clock();
  while (getSonarValue() <= DISTANCE_FROM_OBSTACLE && (clock() - before) / CLOCKS_PER_SEC < 10);
  difference = clock() - before;
  sleep(2);
  stopRunning();
  sec = difference / CLOCKS_PER_SEC;

  if ((!nextToWall() && callsOfdetectObstacles == 1) || callsOfdetectObstacles == 3) {
    callsOfdetectObstacles = 0;
  } else if (callsOfdetectObstacles < 3) {
    printf("Time spent: %d\n", sec);
    detectObstacles();
  }
}
