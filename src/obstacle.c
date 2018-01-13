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

//int length;
//int weidth;

//int calls = 0;

char closeToObstacles() {
  return (getSonarValue() <= DISTANCE_FROM_OBSTACLE);
}

/*char farObstacle() {
  return (getSonarValue() <= (DISTANCE_FROM_OBSTACLE + 300));
}*/

void BasicReaction()
{
  stopRunning();
  turnSonar(-90);
  int rightx = getSonarValue();
  turnSonar(180);
  int leftx = getSonarValue();
  printf("left distance: %d right distance: %d\n", leftx, rightx);
  turnSonar(-90);
  goStraight(500,-1); // have a "safety" distance
  turn((rightx >= leftx ? -1 : 1) * 90);
}

/*

void detectObstacles() {
  turnSonar(180);
  if (closeToObstacles()) {
    printf("An obstacle was found! The distance from this obstacle is: %dmm.\n", getSonarValue());
    printf("COLOR: %s\n", getColorName(getColorValue()));
    updateMapPosition(getSonarValue(), (getColorValue() == 5 ? BALL : OBSTACLE));
    goStraight(0,-1);
    while (closeToObstacles());
    stopRunning();
  }
  sleep(1);
  turnSonar(-180);
goStraight(100,-1);
  actionAfterDetecting();
}

void actionAfterDetecting() {
  int timeInit;
  calls++;
  turnSonar(-90);
  if (calls==3 && closeToObstacles()){calls=1;}
  turn(((calls == 1) ? 1 : -1) * 90);
  timeInit = clock() / CLOCKS_PER_SEC;
  goStraight(0,1);
//sleep(1);
  while (closeToObstacles() || calls==2) {
    turnSonar(90);
    if (farObstacle() || (clock() / CLOCKS_PER_SEC - timeInit > 1)) {
      stopRunning();
      calls = 0;
      return;
    }
    turnSonar(-90);
  }
  sleep(1);
  stopRunning();

  if (calls == 3) {
    turn(90);
    calls = 0;
  } else if (calls < 3) {
    turnSonar(90);
    actionAfterDetecting();
  }
}
*/
