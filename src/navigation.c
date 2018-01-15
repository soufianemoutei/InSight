#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "engines.h"
#include "sensors.h"
#include "position.h"
#include "map.h"
#include "obstacle.h"
#include "main.h"
#include "navigation.h"

char exploring = 1;

void explore() {

  initPosition(STARTING_POSITION_X*5, STARTING_POSITION_Y*5); // Initializing the position

  snake(); // Use the ‘snake’ strategy
  // strategy_beta();
  // exploreUsingLayers();

  exploring = 0;
  freePosition(); // Stop updating the position

  correctMap();
  send_map(); // The map is ready to be sent to the server.
}

void snake()
{
  int ob = 0; // For checking consecutive obstacles
  int round = 1;
  int i = 0; // The number of iterations
  int timeInit = clock() / CLOCKS_PER_SEC; // The initial time. It will be used to work out the time spent in exploring the arena.
  int16_t pos[2]; // It will be used to store the position of the robot in case it releases an obstacle
  int rounds = 0; // 1st round, the robot will explore horizontally. 2nd round, the robot will explore vertically. After that, it will stop exploring.
  int angle, error = 0; // To correct the error accumulated while running

  goStraight(500,1); // To avoid bumping the robot's back into the walls
  turn(-90); // Turn to the angle 0° to explore the area horizontally.

  while (exploring && (clock() / CLOCKS_PER_SEC - timeInit < 200) && rounds < 4) {
    if (i % 2 == 1) { // Correct the heading every two iterations.
      correctHeading();
    }

    angle = getGyroValue();
    goStraight(10000,1);

    while (!closeToObstacles() && onTheMap()); // Stop if the robot sees an obstacle or if it's out of the arena.

    stopRunning(); // Stop running to deal with the situation (Finding an obstacle or getting out of the arena)

    // Correct the error
    error = getGyroValue() - angle;
    if (abs(error) > 20) {
      turn(-error);
    }

    pthread_mutex_lock(&turningMutex); // Stop turning the sonar sensor in the thread

    if (closeToObstacles()) {
      if (!isBall()) {
        updateMapPosition(getSonarValue()); // Add the obstacle to the map if it isn't movable.
      }

      fprintf(printFile,"An obstacle was found! The distance from this obstacle is: %dmm; its color is %s. Consecutive obstacles: %d.\n", getSonarValue(), getColorName(getColorValue()), ob);
    }

    if (!onTheMap()) {
      pthread_mutex_lock(&positionMutex);
      outTheMap = 0;
      pthread_mutex_unlock(&positionMutex);

      goStraight(1500,-1); // Run backward to return to the arena if the robot is out of it.
    } else {
      goStraight(((ob >= 2) ? 1000 : 750),-1); // Run backward for some milliseconds before turning to an another side.
    }

    turn(round*90); // Quarter-turn to avoid the obstacle OR to avoid leaving the arena

    if (obstaclesToRelease > 0) { // If there are obstacles to release
      backEngine(1); // Release an obstacle
      getPosition(pos);
      send_obstacle(pos[0], pos[1]); // Inform the server that the robot has just released an obstacle
      obstaclesToRelease--;
    }

    if (!closeToObstacles()) {
      goStraight(850,1); // Go to the next line to explore it.
      ob = 0;
    } else {

      if (!isBall()) {
        updateMapPosition(getSonarValue()); // Add the obstacle to the map if it's movable.
      }

      fprintf(printFile,"An obstacle was found! The distance from this obstacle is: %dmm; its color is %s. OB = %d.\n", getSonarValue(), getColorName(getColorValue()), ob);
      ob++;
    }

    turn(round*90); // Quarter-turn to explore the new line.

    if (ob >= 2){
      fprintf(printFile,"WARNING: Limit of consecutive obstacles is reached: %d/2.\n",ob);
      turn(round*90); // Quarter-turn to get out of the corner
      rounds++;
    }
    round = -round;
    pthread_mutex_unlock(&turningMutex); // Continue turning the sonar sensor in the thread
    i++;
  }
}

void strategy_beta()
{
  int timeInit = clock() / CLOCKS_PER_SEC; // The initial time. It will be used to work out the time spent in exploring the arena.
  int i = 0; // The number of iterations
  int16_t pos[2]; // It will be used to store the position of the robot in case it releases an obstacle


  while (exploring && (clock() / CLOCKS_PER_SEC - timeInit < 180)) {
    if (i % 2 == 1) { // Correct the heading every two iterations.
      correctHeading();
    }

    goStraight(0,1);

    while (!closeToObstacles() && onTheMap()); // Stop if the robot sees an obstacle or if it's out of the arena.

    pthread_mutex_lock(&turningMutex); // Stop turning the sonar sensor in the thread

    stopRunning(); // Stop running to deal with the situation (Finding an obstacle or getting out of the arena)

    if (closeToObstacles()) {
      if (!isBall()) {
        updateMapPosition(getSonarValue()); // Add the obstacle to the map if it's movable.
      }

      fprintf(printFile,"An obstacle was found! The distance from this obstacle is: %dmm; its color is %s.\n", getSonarValue(), getColorName(getColorValue()));
    }

    if (!onTheMap()) {
      pthread_mutex_lock(&positionMutex);
      outTheMap = 0;
      pthread_mutex_unlock(&positionMutex);

      goStraight(1500,-1); // Run a backward to return to the arena if the robot is out of it.
    }

    BasicReaction();

    if (obstaclesToRelease > 0) { // If there are obstacles to release
      backEngine(1); // Release an obstacle
      getPosition(pos);
      send_obstacle(pos[0], pos[1]); // Inform the server that the robot has just released an obstacle
      obstaclesToRelease--;
    }

    pthread_mutex_unlock(&turningMutex); // Continue turning the sonar sensor in the thread
    i++;
  }
}

void exploreUsingLayers() {
  int timeInit = clock() / CLOCKS_PER_SEC; // The initial time. It will be used to work out the time spent in exploring the arena.
  int i = 1;

  while (exploring && i <= 5 && (clock() / CLOCKS_PER_SEC - timeInit < 180)) {
    exploreLayer(i); // Explore the i-th layer
    i++;
  }
}

void exploreLayer(int currentLayerID) {
  int16_t posInit[2], pos[2];

  getPosition(posInit);
  fprintf(printFile,"Exploring Layer %d: Starting position (%d,%d).\n",currentLayerID,posInit[0],posInit[1]);

  turn(-90); // Start exploring the layer from the robot's right

  do {
    goStraight(10000,1);

    while (!closeToObstacles() && onTheMap()); // Stop if the robot sees an obstacle or if it's out of the arena.

    pthread_mutex_lock(&turningMutex); // Stop turning the sonar sensor in the thread

    stopRunning(); // Stop running to deal with the situation (Finding an obstacle or getting out of the arena)

    if (closeToObstacles()) {
      if (!isBall()) {
        updateMapPosition(getSonarValue()); // Add the obstacle to the map if it's movable.
      }

      fprintf(printFile,"An obstacle was found! The distance from this obstacle is: %dmm; its color is %s.\n", getSonarValue(), getColorName(getColorValue()));
    }

    if (!onTheMap()) {
      pthread_mutex_lock(&positionMutex);
      outTheMap = 0;
      pthread_mutex_unlock(&positionMutex);

      goStraight(1500,-1); // Run a backward to return to the arena if the robot is out of it.
    }

    if (obstaclesToRelease > 0) { // If there are obstacles to release
      backEngine(1); // Release an obstacle
      getPosition(pos);
      send_obstacle(pos[0], pos[1]); // Inform the server that the robot has just released an obstacle
      obstaclesToRelease--;
    }

    fourTurnsAction(1); // Go to the other side of the obstacle

    pthread_mutex_unlock(&turningMutex); // Continue turning the sonar sensor in the thread

    getPosition(pos);
    fprintf(printFile,"Exploring Layer %d: Current position (%d,%d).\n",currentLayerID,pos[0],pos[1]);

    while ((pos[0] < MAP_WIDTH-currentLayerID) || (pos[1] < MAP_HEIGHT-currentLayerID)) { // To make the robot return to layer ‘currentLayerID’.
      goStraight(750,-1);
      turn(90);
    }

  } while (!(pos[0] == posInit[0] && pos[1] == posInit[1])); // Until the robot reaches again its initial position.

  turn(90); // Return to the initial heading
  goStraight(4000,1); // Change the layer
  do {
    getPosition(pos);
  } while (pos[1] != posInit[1]);
  stopRunning();
}
