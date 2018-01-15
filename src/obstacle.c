#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_sensor.h"
#include "ev3_tacho.h"
#include "engines.h"
#include "sensors.h"
#include "obstacle.h"
#include "position.h"
#include "main.h"

char closeToObstacles() {
  return (getSonarValue() <= DISTANCE_FROM_OBSTACLE);
}

char isBall() {
  // /*
  int color = getColorValue(); // If it equals 5, the color is red.

  turnSonar(-10);
  int colorRight = getColorValue();

  turnSonar(20);
  int colorLeft = getColorValue();

  return (color == 5 && colorRight != 5 && colorLeft != 5); // The obstacle is a red ball and not a red non-movable obstacle

  // */

  //return (getColorValue() == 5);
}

/* BEGIN: A strategy to avoid an obstacle by going to the more vacant direction */
void BasicReaction()
{
  turnSonar(-45); // Check the right of the robot
  int right = getSonarValue();

  turnSonar(90); // Check the left of the robot
  int left = getSonarValue();

  fprintf(printFile,"OBSTACLE: left distance is %d, right distance is %d.\n", left, right);

  turnSonar(-45); // Turn the sonar sensor to its initial position (before BasicReaction())

  goStraight(500,-1); // Running backward in order to have a "safety" distance.

  turn((right >= left ? -1 : 1) * 90); // Turn right if there is more space on the right of the robot than its left; turn left otherwise.
}
/* END: A strategy to avoid an obstacle by going to the more vacant direction */

/* BEGIN: Another strategy to avoid an obstacle by going to its other side. Hence, the function name. */
void fourTurnsAction(int calls) {
  int timeInit;

  if (calls == 1 || calls == 4) {
    // For the first and the last calls, check if the robot's left is empty before turning 90° degrees.
    checkBeforeTurn();
    turn(90);
  } else {
    // Turn -90° degrees twice to avoid the obstacle.
    turn(-90);
  }

  if (calls < 4) { // If the robot is still dealing with the obstacle.
    turnSonar(-90); // See the robot's right to see the obstacle

    timeInit = clock() / CLOCKS_PER_SEC;
    goStraight(0,1); // Run until the robot is no longer seeing the obstacle, or if it's seeing the obstacle for more than 5 seconds to conclude the latter is a border of the arena.
    while (closeToObstacles() && (clock() / CLOCKS_PER_SEC - timeInit < 5));

    sleep(1);
    stopRunning();

    if (clock() / CLOCKS_PER_SEC - timeInit >= 10) {
      // If it's a border of the map, continue running.
      return ;
    } // Else, continue avoiding the obstacle

    turnSonar(90); // Turn the sonar sensor to its initial position (before fourTurnsAction())

    fourTurnsAction(calls+1); // Recall the function for the (calls+1)-th time.
  }
}

void checkBeforeTurn() {
  turnSonar(90); // See the robot's left

  if (closeToObstacles()) { // If there is an obstacle on the left of thr robot
    if (!isBall()) {
      updateMapPosition(getSonarValue()); // Add the obstacle to the map if it's movable.
    }

    fprintf(printFile,"An obstacle was found on the left of the robot! The distance from this obstacle is: %dmm; its color is %s.\n", getSonarValue(), getColorName(getColorValue()));

    goStraight(0,-1); // Run backward until the robot is not seeing the obstacle anymore.
    while (closeToObstacles());
    stopRunning();
  }


  turnSonar(-90); // Turn the sonar sensor to its initial position (before checkBeforeTurn())
}
/* END: Another strategy to avoid an obstacle by going to the its other side. Hence, the function name. */
