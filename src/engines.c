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

void initEng()
{
  printf("Engine initialization...\n");

  while (ev3_tacho_init() < 1) sleep(1);

  if (ev3_search_tacho_plugged_in(RIGHT_PORT,0,&(engines.wheelEng.right),0) && ev3_search_tacho_plugged_in(LEFT_PORT,0,&(engines.wheelEng.left),0)) {
    printf("Wheels' engines were found!\n");
    engines.wheelEng.both[0] = engines.wheelEng.right;
    engines.wheelEng.both[1] = engines.wheelEng.left;

    set_tacho_stop_action_inx(engines.wheelEng.right, TACHO_RESET);
    set_tacho_stop_action_inx(engines.wheelEng.left, TACHO_RESET);
    multi_set_tacho_position_sp(engines.wheelEng.both, 0);
    multi_set_tacho_position(engines.wheelEng.both, 0);
  } else {
    printf("Wheels' engines were NOT found!\n");
  }

  if (ev3_search_tacho_plugged_in(UP_PORT,0,&engines.upEng,0)) {
    printf("UP engine were found!\n");
    set_tacho_stop_action_inx(engines.upEng, TACHO_COAST);
  } else {
    printf("UP engine were NOT found\n");
  }
}

void goStraight(int time) {
  multi_set_tacho_stop_action_inx(engines.wheelEng.both, TACHO_COAST);
  multi_set_tacho_speed_sp(engines.wheelEng.both, SPEED);
  multi_set_tacho_ramp_up_sp(engines.wheelEng.both, 100);  // 0.1 second to reach the full speed
  multi_set_tacho_ramp_down_sp(engines.wheelEng.both, 100);// 0.1 is the acceleration and decceleration time

  if (!time) {
    multi_set_tacho_command_inx(engines.wheelEng.both, TACHO_RUN_FOREVER);
  } else {
    multi_set_tacho_time_sp(engines.wheelEng.both, time);
    multi_set_tacho_command_inx(engines.wheelEng.both, TACHO_RUN_TIMED);
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

  int s = (degree > 0 ? 1 : -1);
  degree = s * degree;

  int halftime = 2265; // the time for 180 degree
  int time = degree * halftime / 180;

  set_tacho_speed_sp(engines.wheelEng.right, s*SPEED);
  set_tacho_speed_sp(engines.wheelEng.left, -s*SPEED);

  multi_set_tacho_ramp_up_sp(engines.wheelEng.both, 100);  // 0.1 second to reach the full speed
  multi_set_tacho_ramp_down_sp(engines.wheelEng.both, 100);// 0.1 is the acceleration and decceleration time
  multi_set_tacho_time_sp(engines.wheelEng.both, time);
  multi_set_tacho_command_inx(engines.wheelEng.both, TACHO_RUN_TIMED);
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

void upAction() {
  printf("UP:\n");
  set_tacho_speed_sp(engines.upEng, -SPEED/2);
  set_tacho_ramp_up_sp(engines.upEng, 100 );  // 0.1 second to reach the full speed
  set_tacho_ramp_down_sp(engines.upEng, 100 );// 0.1 is the acceleration and decceleration time
  set_tacho_time_sp(engines.upEng, 2000);
  set_tacho_command_inx(engines.upEng, TACHO_RUN_TIMED);
  sleep(2);
}

void downAction() {
  printf("DOWN:\n");
  set_tacho_speed_sp(engines.upEng, SPEED/2);
  set_tacho_ramp_up_sp(engines.upEng, 100 );  // 0.1 second to reach the full speed
  set_tacho_ramp_down_sp(engines.upEng, 100 );// 0.1 is the acceleration and decceleration time
  set_tacho_time_sp(engines.upEng, 2000);
  set_tacho_command_inx(engines.upEng, TACHO_RUN_TIMED);
  sleep(2);
}

void explore() {
  initPosition(40.0,10.0);
  turn(90);
  sleep(2);
  goStraight(0);
  detectObstacles();
  sleep(30);
  stopRunning();
  freePosition();
}
