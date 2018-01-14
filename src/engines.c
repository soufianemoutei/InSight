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

pthread_mutex_t turningMutex;
pthread_t movingEyesThread;

void initEng()
{
  printf("Engine initialization...\n");

  while (ev3_tacho_init() < 1) sleep(1);

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
    exploring = 0;
    printf("Wheels' engines were NOT found!\n");
  }

  if (ev3_search_tacho_plugged_in(BACK_PORT,0,&engines.backEng,0)) {
    printf("BACK engine were found!\n");
    set_tacho_stop_action_inx(engines.backEng, TACHO_RESET);
    set_tacho_ramp_up_sp(engines.backEng, 100);  // 0.1 second to reach the full speed
    set_tacho_ramp_down_sp(engines.backEng, 100);// 0.1 is the decceleration time
  } else {
    engines.backEng = DESC_LIMIT;
    printf("BACK engine were NOT found\n");
  }

  if (ev3_search_tacho_plugged_in(FRONT_PORT,0,&engines.frontEng,0)) {
    printf("FRONT engine were found!\n");
    set_tacho_ramp_up_sp(engines.frontEng, 100); // 0.1 second to reach the full speed
    set_tacho_ramp_down_sp(engines.frontEng, 100); // 0.1 is the acceleration and decceleration time

    pthread_mutex_init(&turningMutex, NULL);
    if(pthread_create(&movingEyesThread, NULL, move_eyes, NULL) == -1) {
      printf("pthread_create");
      exit(EXIT_FAILURE);
    }

  } else {
    exploring = 0;
    printf("FRONT engine were NOT found\n");
  }
}

void goStraight(int time, int direction) {
  int angleInit = getGyroValue(), error, timeInit;

  multi_set_tacho_speed_sp(engines.wheelEng.both, direction*SPEED_LINEAR/(direction == -1 ? 2 : 1));
  if (!time) {
    multi_set_tacho_command_inx(engines.wheelEng.both, TACHO_RUN_FOREVER);
  } else {
    multi_set_tacho_time_sp(engines.wheelEng.both, time);

    timeInit = clock() / CLOCKS_PER_SEC;
    multi_set_tacho_command_inx(engines.wheelEng.both, TACHO_RUN_TIMED);
  }
  Sleep(1000);
}

void stopRunning() {
  multi_set_tacho_command_inx(engines.wheelEng.both, TACHO_STOP);
}

int isRunning() {
  FLAGS_T flagsL, flagsR;
  get_tacho_state_flags(engines.wheelEng.left,&flagsL);
  get_tacho_state_flags(engines.wheelEng.right,&flagsR);
  return (flagsL != TACHO_STATE__NONE_ && flagsR != TACHO_STATE__NONE_);
  //return (flags == TACHO_RUNNING);
}

void turn(int degree){
  printf("TURNING BY %d degrees\n", degree);

  multi_set_tacho_speed_sp(engines.wheelEng.both, SPEED_CIRCULAR);
  set_tacho_position_sp(engines.wheelEng.left, DEGREE_TO_COUNT(-degree));
  set_tacho_position_sp(engines.wheelEng.right, DEGREE_TO_COUNT(degree));
  multi_set_tacho_command_inx( engines.wheelEng.both, TACHO_RUN_TO_REL_POS);

  Sleep(1500);
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
  if ((angle > ERROR && angle <= 45) || (angle > 45 && angle < (90-ERROR))){
    printf("Correcting the current heading: %d.\n",angle);
  }
  if (angle > ERROR && angle <= 45) {
    turn(-angle);
  } else if (angle > 45 && angle < (90-ERROR)){
    turn(90-angle);
  }
}

void backEngine(int direction) {
  if (engines.backEng == DESC_LIMIT) {
    printf("CANNOT USE BACK ENGINE\n");
    return ;
  }
  set_tacho_speed_sp(engines.backEng, direction * SPEED_BACK_ENGINE);
  set_tacho_time_sp(engines.backEng, 1000);
  set_tacho_command_inx(engines.backEng, TACHO_RUN_TIMED);
  sleep(2);
}

void turnSonar(int angle) {
  set_tacho_speed_sp(engines.frontEng, (angle < 0 ? -1 : 1) * SPEED_FRONT_ENGINE);
  if(!angle){
    set_tacho_command_inx(engines.frontEng, TACHO_RUN_FOREVER);
  } else {
    set_tacho_time_sp(engines.frontEng,DEGREE_TO_TIME_FE(angle));
    set_tacho_command_inx(engines.frontEng, TACHO_RUN_TIMED);
    Sleep(1500);
  }
}


void explore(){
  int i = 0;
  initPosition(STARTING_POSITION_X*5, STARTING_POSITION_Y*5);

  snake();
  /*while (exploring && i < 50) {
  goStraight(2000,1);
  if (i%4 == 3 && !closeToObstacles()) {
  Sleep(1500);
}
if (closeToObstacles() || i%4 == 3) {
updateMapPosition(getSonarValue(), (getColorValue() == 5 ? VISITED : OBSTACLE));
BasicReaction();
}
i++;
}*/

freePosition();
send_map();
}

void snake()
{
  int ob = 0; //for checking to consecutive obstacles
  int round = 1;
  int i = 0;

  turn(-90);
  while (exploring) {
    if (i % 2 == 1) {
      correctHeading();
      sleep(1);
    }
    goStraight(10000,1);
    while (!closeToObstacles() && onTheMap());

    pthread_mutex_lock(&turningMutex);

    stopRunning();

    if (getColorValue() != 5) {
      updateMapPosition(getSonarValue());
    }
    printf("An obstacle was found! The distance from this obstacle is: %dmm. OB = %d.\n", getSonarValue(), ob);
    printf("COLOR: %s\n", getColorName(getColorValue()));

    goStraight((ob >= 2 ? 750 : 500),-1);

    turn(round*90);

    if (!closeToObstacles()){
      goStraight(500,1);
      ob = 0;
    } else {
      if (getColorValue() != 5) {
        updateMapPosition(getSonarValue());
      }
      printf("An obstacle was found! The distance from this obstacle is: %dmm. OB = %d.\n", getSonarValue(), ob);
      printf("COLOR: %s\n", getColorName(getColorValue()));
      ob++;
    }

    turn(round*90);

    if (ob>=2){
      printf("LIMIT REACHED, OB = %d.\n",ob);
      turn(round*90);
    }
    round = -round;
    pthread_mutex_unlock(&turningMutex);
    i++;
  }
}

void* move_eyes() {
  while (exploring) {
    pthread_mutex_lock(&turningMutex);
    turnSonar(-15);
    pthread_mutex_unlock(&turningMutex);
    Sleep(10);
    pthread_mutex_lock(&turningMutex);
    turnSonar(15);
    pthread_mutex_unlock(&turningMutex);
  }
}
