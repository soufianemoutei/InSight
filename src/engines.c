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

  if (ev3_search_tacho_plugged_in(BACK_PORT,0,&engines.backEng,20)) {
    printf("BACK engine were found!\n");
   // set_tacho_stop_action_inx(engines.backEng, TACHO_COAST);
//set_tacho_command_inx(engines.backEng, TACHO_STOP);
  } else {
    printf("BACK engine were NOT found\n");
  }
  if (ev3_search_tacho_plugged_in(FRONT_PORT,0,&engines.frontEng,20)) {
    printf("FRONT engine were found!\n");
   // set_tacho_stop_action_inx(engines.frontEng, TACHO_COAST);
//set_tacho_command_inx(engines.frontEng, TACHO_STOP);
  } else {
    printf("FRONT engine were NOT found\n");
}
}
void goStraight(int time) {
  int direction = 1;
  if (time == -1){direction = -1;}	
  multi_set_tacho_stop_action_inx(engines.wheelEng.both, TACHO_COAST);
  multi_set_tacho_speed_sp(engines.wheelEng.both, -direction*SPEED);
  multi_set_tacho_ramp_up_sp(engines.wheelEng.both, 100);  // 0.1 second to reach the full speed
  multi_set_tacho_ramp_down_sp(engines.wheelEng.both, 100);// 0.1 is the acceleration and decceleration time

  if (!time|| time == -1) {
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
  multi_set_tacho_speed_sp(engines.wheelEng.both, SPEED);

    set_tacho_position_sp( engines.wheelEng.left, -degree*180/90);
    set_tacho_position_sp( engines.wheelEng.right, degree*180/90);
    multi_set_tacho_command_inx( engines.wheelEng.both, TACHO_RUN_TO_REL_POS );
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
  set_tacho_speed_sp(engines.backEng, -SPEED/2);
  set_tacho_ramp_up_sp(engines.backEng, 100 );  // 0.1 second to reach the full speed
  set_tacho_ramp_down_sp(engines.backEng, 100 );// 0.1 is the acceleration and decceleration time
  set_tacho_time_sp(engines.backEng, 2000);
  set_tacho_command_inx(engines.backEng, TACHO_RUN_TIMED);
  sleep(2);
}

void downAction() {
  printf("DOWN:\n");
  set_tacho_speed_sp(engines.backEng, SPEED/2);
  set_tacho_ramp_up_sp(engines.backEng, 100 );  // 0.1 second to reach the full speed
  set_tacho_ramp_down_sp(engines.backEng, 100 );// 0.1 is the acceleration and decceleration time
  set_tacho_time_sp(engines.backEng, 2000);
  set_tacho_command_inx(engines.backEng, TACHO_RUN_TIMED);
  sleep(2);
}

void turnLeft(int angle) {
    printf("RIGHT:\n");
    set_tacho_speed_sp(engines.frontEng, SPEED/4);
    set_tacho_ramp_up_sp(engines.frontEng, 100 );  // 0.1 second to reach the full speed
    set_tacho_ramp_down_sp(engines.frontEng, 100 );// 0.1 is the acceleration and decceleration time
  if(!angle){
    set_tacho_command_inx(engines.frontEng, TACHO_RUN_FOREVER);
  }else {
    set_tacho_time_sp(engines.frontEng, 2500);
    set_tacho_command_inx(engines.frontEng, TACHO_RUN_TIMED);
     sleep(2);
   }
}

void turnRight(int angle) {
	printf("LEFT:\n");
	set_tacho_speed_sp(engines.frontEng, -SPEED/4);
	set_tacho_ramp_up_sp(engines.frontEng, 100 );  // 0.1 second to reach the full speed
    set_tacho_ramp_down_sp(engines.frontEng, 100 );// 0.1 is the acceleration and decceleration time

   if(!angle){
    set_tacho_command_inx(engines.frontEng, TACHO_RUN_FOREVER);
  }else {
    set_tacho_time_sp(engines.frontEng, 2500);
    set_tacho_command_inx(engines.frontEng, TACHO_RUN_TIMED);
    sleep(2);
  }
}

void explore() {
  initPosition(40.0,10.0);
  turn(-90);
  sleep(2);
  
  while(1){
    if(getSonarValue() <= 100){
      detectObstaclesF();
      detectObstaclesB();
      detectObstaclesB();
      detectObstaclesB();
      if (getSonarValue() > 100) {
        goStraight(0); 
      }
    }
  }
  freePosition();
}

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
