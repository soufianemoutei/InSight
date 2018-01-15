#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "engines.h"
#include "sensors.h"
#include "position.h"
#include "obstacle.h"
#include "client.h"
#include <time.h>

Engines engines;
char exploring = 1;
int directionOfSonarSensor = 1;
int obstaclesToRelease = 1;

pthread_mutex_t turningMutex;
pthread_t movingEyesThread;

// The documentation of some tacho functions: http://in4lio.github.io/ev3dev-c/group__ev3__tacho.html

void initEng()
{
  printf("Engine initialization...\n");

  while (ev3_tacho_init() < 1) sleep(1);

  // Initializing the wheels' engines
  if (ev3_search_tacho_plugged_in(RIGHT_PORT,0,&(engines.wheelEng.right),0) && ev3_search_tacho_plugged_in(LEFT_PORT,0,&(engines.wheelEng.left),0)) {
    printf("Wheels' engines were found!\n");

    engines.wheelEng.both[0] = engines.wheelEng.right;
    engines.wheelEng.both[1] = engines.wheelEng.left;
    engines.wheelEng.both[2] = DESC_LIMIT;

    multi_set_tacho_stop_action_inx(engines.wheelEng.both, TACHO_BRAKE);
    multi_set_tacho_position_sp(engines.wheelEng.both, 0);
    multi_set_tacho_position(engines.wheelEng.both, 0);
    multi_set_tacho_ramp_up_sp(engines.wheelEng.both, 1000);  // 1 second to reach the full speed
    multi_set_tacho_ramp_down_sp(engines.wheelEng.both, 500);// 0.5 is the decceleration time
  } else {
    exploring = 0; // if (exploring = 0), the robot won't explore the arena.
    engines.wheelEng.both[0] = DESC_LIMIT;
    engines.wheelEng.both[1] = DESC_LIMIT;
    engines.wheelEng.both[2] = DESC_LIMIT;
    printf("Wheels' engines were NOT found!\n");
  }

  // Initializing the back engine
  if (ev3_search_tacho_plugged_in(BACK_PORT,0,&engines.backEng,0)) {
    printf("BACK engine were found!\n");

    set_tacho_stop_action_inx(engines.backEng, TACHO_RESET);
    set_tacho_ramp_up_sp(engines.backEng, 100);  // 0.1 second to reach the full speed
    set_tacho_ramp_down_sp(engines.backEng, 100);// 0.1 is the decceleration time
  } else {
    engines.backEng = DESC_LIMIT;
    printf("BACK engine were NOT found\n");
  }

  // Initializing the front engine
  if (ev3_search_tacho_plugged_in(FRONT_PORT,0,&engines.frontEng,0)) {
    printf("FRONT engine were found!\n");

    set_tacho_ramp_up_sp(engines.frontEng, 100); // 0.1 second to reach the full speed
    set_tacho_ramp_down_sp(engines.frontEng, 100); // 0.1 is the decceleration time

    pthread_mutex_init(&turningMutex, NULL);
    if (pthread_create(&movingEyesThread, NULL, move_eyes, NULL) == -1) { // Start the thread using ‘move_eyes’ function
      printf("pthread_create");
      exit(EXIT_FAILURE);
    }

  } else {
    engines.frontEng = DESC_LIMIT;
    printf("FRONT engine were NOT found\n");
  }
}

/* BEGIN: The wheels' engines functions */
void goStraight(int time, int direction) {
  multi_set_tacho_speed_sp(engines.wheelEng.both, direction*SPEED_LINEAR/(direction == -1 ? 2 : 1)); // Use half of SPEED_LINEAR if the robot will run backward.

  if (!time) {
    multi_set_tacho_command_inx(engines.wheelEng.both, TACHO_RUN_FOREVER);
  } else {
    multi_set_tacho_time_sp(engines.wheelEng.both, time);
    multi_set_tacho_command_inx(engines.wheelEng.both, TACHO_RUN_TIMED);
  }

  Sleep(1000);
}

void turn(int degree){
  printf("Turning by %d° degrees.\n", degree);

  multi_set_tacho_speed_sp(engines.wheelEng.both, SPEED_CIRCULAR);
  set_tacho_position_sp(engines.wheelEng.left, DEGREE_TO_COUNT(-degree));
  set_tacho_position_sp(engines.wheelEng.right, DEGREE_TO_COUNT(degree));
  multi_set_tacho_command_inx( engines.wheelEng.both, TACHO_RUN_TO_REL_POS);

  Sleep(1500);
}

void stopRunning() {
  multi_set_tacho_command_inx(engines.wheelEng.both, TACHO_STOP);
}

char isRunning() {
  FLAGS_T flagsL, flagsR;
  get_tacho_state_flags(engines.wheelEng.left,&flagsL);
  get_tacho_state_flags(engines.wheelEng.right,&flagsR);
  return (flagsL != TACHO_STATE__NONE_ && flagsR != TACHO_STATE__NONE_);
  //return (flags == TACHO_RUNNING);
}

int leftWheelPosition() {
  int buf;
  get_tacho_position(engines.wheelEng.left,&buf);
  return buf;
}

int rightWheelPosition() {
  int buf;
  get_tacho_position(engines.wheelEng.right,&buf);
  return buf;
}

void correctHeading() {
  int angle = getHeading() % 90;

  if (angle > ERROR && angle <= 45) {
    printf("Correcting the current heading %d° by turning %d°.\n",angle,-angle);
    turn(-angle);
  } else if (angle > 45 && angle < (90-ERROR)){
    printf("Correcting the current heading %d° by turning %d°.\n",angle,90-angle);
    turn(90-angle);
  }

  Sleep(1000);
}

/* END: The wheels' engines functions */

/* BEGIN: The back engine functions */
void backEngine(int direction) {
  if (engines.backEng == DESC_LIMIT) {
    printf("ERROR: CANNOT USE BACK ENGINE!\n");
    return ;
  }

  set_tacho_speed_sp(engines.backEng, direction * SPEED_BACK_ENGINE);
  set_tacho_time_sp(engines.backEng, 1000);
  set_tacho_command_inx(engines.backEng, TACHO_RUN_TIMED);

  Sleep(2000);
}
/* END: The back engine functions */

/* BEGIN: The front engine functions */
void turnSonar(int angle) {
  if (engines.frontEng == DESC_LIMIT) {
    printf("ERROR: CANNOT USE FRONT ENGINE!\n");
    return ;
  }

  set_tacho_speed_sp(engines.frontEng, (angle < 0 ? -1 : 1) * SPEED_FRONT_ENGINE);

  if (!angle) {
    set_tacho_command_inx(engines.frontEng, TACHO_RUN_FOREVER);
  } else {
    set_tacho_time_sp(engines.frontEng,DEGREE_TO_TIME_FE(angle));
    set_tacho_command_inx(engines.frontEng, TACHO_RUN_TIMED);
    Sleep(1000);
  }
}

void* move_eyes() {
  while (exploring) {
    pthread_mutex_lock(&turningMutex);
    turnSonar(directionOfSonarSensor * ANGLE_THREAD_SONAR);
    turnSonar(-directionOfSonarSensor * ANGLE_THREAD_SONAR);
    directionOfSonarSensor = -1;
    pthread_mutex_unlock(&turningMutex);
    Sleep(10);
  }
}
/* END: The front engine functions */

/* BEGIN: The exploring functions */
void explore(){

  initPosition(STARTING_POSITION_X*5, STARTING_POSITION_Y*5); // Initializing the position

  snake(); // Use the ‘snake’ strategy
  // strategy_beta();

  freePosition(); // Stop updating the position
  send_map(); // The map is ready to be sent to the server.
}

void snake()
{
  int ob = 0; // For checking consecutive obstacles
  int round = 1;
  int i = 0; // The number of iterations
  int timeInit = clock() / CLOCKS_PER_SEC; // The initial time. It will be used to work out the time spent in exploring the arena.
  int16_t pos[2]; // It will be used to store the position of the robot in case it releases an obstacle

  turn(-90); // Turn to the angle 0° to explore the area horizontally.

  while (exploring && (clock() / CLOCKS_PER_SEC - timeInit < 180)) {
    if (i % 2 == 1) { // Correct the heading every two iterations.
      correctHeading();
    }

    goStraight(10000,1);

    while (!closeToObstacles() && onTheMap()); // Stop if the robot sees an obstacle or if it's out of the arena.

    pthread_mutex_lock(&turningMutex); // Stop turning the sonar sensor in the thread

    stopRunning(); // Stop running to deal with the situation (Finding an obstacle or getting out of the arena)

    if (closeToObstacles()) {
      if (!isBall()) {
        updateMapPosition(getSonarValue()); // Add the obstacle to the map if it's movable.
      }

      printf("An obstacle was found! The distance from this obstacle is: %dmm; its color is %s. Consecutive obstacles: %d.\n", getSonarValue(), getColorName(getColorValue()), ob);
    }

    goStraight(((ob >= 2 || !onTheMap()) ? 750 : 500),-1); // Run a backward for some milliseconds before turning to an another side; to return to the arena if the robot is out of it.
    turn(round*90); // Quarter-turn to avoid the obstacle OR to avoid leaving the arena

    if (obstaclesToRelease > 0) { // If there are obstacles to release
      backEngine(1); // Release an obstacle
      getPosition(pos);
      send_obstacle(pos[0], pos[1]); // Inform the server that the robot has just released an obstacle
      //backEngine(0);
      obstaclesToRelease--;
    }

    if (!closeToObstacles()) {
      goStraight(500,1); // Go to the next line to explore it.
      ob = 0;
    } else {

      if (!isBall()) {
        updateMapPosition(getSonarValue()); // Add the obstacle to the map if it's movable.
      }

      printf("An obstacle was found! The distance from this obstacle is: %dmm; its color is %s. OB = %d.\n", getSonarValue(), getColorName(getColorValue()), ob);
      ob++;
    }

    turn(round*90); // Quarter-turn to explore the new line.

    if (ob >= 2){
      printf("WARNING: Limit of consecutive obstacles is reached: %d/2.\n",ob);
      turn(round*90); // Quarter-turn to get out of the corner
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


  while (exploring && (clock() / CLOCKS_PER_SEC - timeInit < 180)) {
    if (i % 2 == 1) { // Correct the heading every two iterations.
      correctHeading();
    }

    goStraight(10000,1);

    while (!closeToObstacles() && onTheMap()); // Stop if the robot sees an obstacle or if it's out of the arena.

    pthread_mutex_lock(&turningMutex); // Stop turning the sonar sensor in the thread

    stopRunning(); // Stop running to deal with the situation (Finding an obstacle or getting out of the arena)

    if (closeToObstacles()) {
      if (!isBall()) {
        updateMapPosition(getSonarValue()); // Add the obstacle to the map if it's movable.
      }

      printf("An obstacle was found! The distance from this obstacle is: %dmm; its color is %s.\n", getSonarValue(), getColorName(getColorValue()));
    }

    if (!onTheMap()) {
      goStraight(750,-1); // Run a backward to return to the arena if the robot is out of it.
    }

    BasicReaction();

    if (obstaclesToRelease > 0) { // If there are obstacles to release
      backEngine(1); // Release an obstacle
      getPosition(pos);
      send_obstacle(pos[0], pos[1]); // Inform the server that the robot has just released an obstacle
      //backEngine(0);
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
  printf("Exploring Layer %d: Starting position (%d,%d).\n",currentLayerID,posInit[0],posInit[1]);

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

      printf("An obstacle was found! The distance from this obstacle is: %dmm; its color is %s.\n", getSonarValue(), getColorName(getColorValue()));
    }

    if (!onTheMap()) {
      goStraight(750,-1); // Run a backward to return to the arena if the robot is out of it.
    }

    if (obstaclesToRelease > 0) { // If there are obstacles to release
      backEngine(1); // Release an obstacle
      getPosition(pos);
      send_obstacle(pos[0], pos[1]); // Inform the server that the robot has just released an obstacle
      //backEngine(0);
      obstaclesToRelease--;
    }

    fourTurnsAction(1); // Go to the other side of the obstacle

    pthread_mutex_unlock(&turningMutex); // Continue turning the sonar sensor in the thread

    getPosition(pos);
    printf("Exploring Layer %d: Current position (%d,%d).\n",currentLayerID,pos[0],pos[1]);

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

/* END: The exploring functions */
