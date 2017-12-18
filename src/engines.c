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

Engines engines;
int runningDirection = 1; // Positive:1. Else, -1

const char const *color[] = { "?", "BLACK", "BLUE", "GREEN", "YELLOW", "RED", "WHITE", "BROWN" };
#define COLOR_COUNT  (( int )( sizeof( color ) / sizeof( color[ 0 ])))

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
    multi_set_tacho_ramp_up_sp(engines.wheelEng.both, 100);  // 0.1 second to reach the full speed
    multi_set_tacho_ramp_down_sp(engines.wheelEng.both, 100);// 0.1 is the acceleration and decceleration time
  } else {
    printf("Wheels' engines were NOT found!\n");
  }

  if (ev3_search_tacho_plugged_in(BACK_PORT,0,&engines.backEng,0)) {
    printf("BACK engine were found!\n");
    set_tacho_stop_action_inx(engines.backEng, TACHO_RESET);
    set_tacho_ramp_up_sp(engines.backEng, 100 );  // 0.1 second to reach the full speed
    set_tacho_ramp_down_sp(engines.backEng, 100 );// 0.1 is the acceleration and decceleration time
  } else {
    printf("BACK engine were NOT found\n");
  }
  if (ev3_search_tacho_plugged_in(FRONT_PORT,0,&engines.frontEng,0)) {
    printf("FRONT engine were found!\n");
    //set_tacho_stop_action_inx(engines.frontEng, TACHO_RESET);
    set_tacho_ramp_up_sp(engines.frontEng, 100 );  // 0.1 second to reach the full speed
    set_tacho_ramp_down_sp(engines.frontEng, 100 );// 0.1 is the acceleration and decceleration time
    if(pthread_create(&movingEyesThread, NULL, move_eyes, NULL) == -1) {
      printf("pthread_create");
      exit(EXIT_FAILURE);
    }
  } else {
    printf("FRONT engine were NOT found\n");
  }
}
void goStraight(int time) {
  int direction = ((time == -1) ? -1 : 1);
  multi_set_tacho_speed_sp(engines.wheelEng.both, -direction*SPEED);
  if (!time || time == -1) {
    multi_set_tacho_command_inx(engines.wheelEng.both, TACHO_RUN_FOREVER);
  } else {
    multi_set_tacho_time_sp(engines.wheelEng.both, time);
    multi_set_tacho_command_inx(engines.wheelEng.both, TACHO_RUN_TIMED);
  }
}

void* move_eyes() {
  while (1) {
    turnSonar(-1);
    sleep(2);
    turnSonar(1);
    sleep(2);
  }
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
  multi_set_tacho_speed_sp(engines.wheelEng.both, SPEED);
  set_tacho_position_sp( engines.wheelEng.left, -degree*180/90);
  set_tacho_position_sp( engines.wheelEng.right, degree*180/90);
  multi_set_tacho_command_inx( engines.wheelEng.both, TACHO_RUN_TO_REL_POS);
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

void backEngine(char direction) {
  printf("Back engine:\n");
  set_tacho_speed_sp(engines.backEng, direction * SPEED/2);
  set_tacho_time_sp(engines.backEng, 1000);
  set_tacho_command_inx(engines.backEng, TACHO_RUN_TIMED);
  sleep(2);
}

void turnSonar(int angle) {
  printf("RIGHT:\n");
  set_tacho_speed_sp(engines.frontEng, (angle < 0 ? 1 : -1) * SPEED/4);
  if(!angle){
    set_tacho_command_inx(engines.frontEng, TACHO_RUN_FOREVER);
  }else {
    set_tacho_time_sp(engines.frontEng, 1500);
    set_tacho_command_inx(engines.frontEng, TACHO_RUN_TIMED);
    sleep(2);
  }
}

void explore() {
  int i = 0;
  initPosition(40.0,10.0);
  turn(-90);
  sleep(2);
  while (1) {
    goStraight(0);
    if (getSonarValue() <= 100) {
      stopRunning();
      printf("An obstacle was found! The distance from this obstacle is: %dmm.\n", getSonarValue());
      turn(runningDirection*90);
      sleep(2);
      goStraight(1000);
      turn(runningDirection*90);
      runningDirection = -runningDirection
      sleep(2);
    }
    i++;
  }
  if (pthread_join(movingEyesThread, NULL)) {
    perror("pthread_join");
    exit(EXIT_FAILURE);
  }
  freePosition();
}

/*
void explore() {
  int i = 0;
  initPosition(40.0,10.0);
  turn(-90);
  sleep(2);
  while (1) {
    if (i == 2) {
      backEngine(1);
    }
    goStraight(0);
    if (getSonarValue() <= 50) {
      stopRunning();
      printf("An obstacle was found! The distance from this obstacle is: %dmm.\n", getSonarValue());
      printf("\r(%s) \n", color[getColorValue()]);
      if (getColorValue() == 5) {
        printf("RED BALL\n");
      }
      turnSonar(90);
      sleep(2);
      detectObstacles();
      sleep(2);
      turnSonar(-90);
    }
    i++;
  }
  freePosition();
}
*/
void exploreSmallArena(){
  initPosition(60.0, 20.0); // the starting position of the robot is in the center back of the arena
  for (int i = 1; i <= 4; i++){
    goToNextCheckpoint(i);
  }
  goToNextCheckpoint(0);
  changeLayer();
}

void goToNextCheckpoint(int currentCpId){ // a checkpoint is a corner of a layer of the arena
  printf("Current Checkpoint ID = %d\n",currentCpId);
  while(1){
    goStraight(0);
    // getPosition(x,y); // I need to specify the x and y
    //if(FinalPositionReached){ //
    // break;
    //}
    int val = getSonarValue();
    if(val<200){ // the reaction of the robot in front of an obsatcle
      turn(90);
      goStraight(2000); //The input value of goStraight has to be adapted according to the Obstacle
      turn(90);
      goStraight(2000);
      turn(-90);
      goStraight(2000);
      turn(-90);
    }
  }
}

void changeLayer(void){
  goStraight(1500);
  turn(-90);
}
