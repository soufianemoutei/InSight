#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "engines.h"
#include "main.h"
#include "navigation.h"

Engines engines;
int directionOfSonarSensor = 1;
int obstaclesToRelease = 0;

pthread_mutex_t turningMutex;
pthread_t movingEyesThread;

// The documentation of some tacho functions: http://in4lio.github.io/ev3dev-c/group__ev3__tacho.html

void initEng()
{
  fprintf(printFile,"Engine initialization...\n");

  while (ev3_tacho_init() < 1) sleep(1);

  // Initializing the wheels' engines
  if (ev3_search_tacho_plugged_in(RIGHT_PORT,0,&(engines.wheelEng.right),0) && ev3_search_tacho_plugged_in(LEFT_PORT,0,&(engines.wheelEng.left),0)) {
    fprintf(printFile,"Wheels' engines were found!\n");

    engines.wheelEng.both[0] = engines.wheelEng.right;
    engines.wheelEng.both[1] = engines.wheelEng.left;
    engines.wheelEng.both[2] = DESC_LIMIT;

    multi_set_tacho_command_inx(engines.wheelEng.both, TACHO_RESET);
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
    fprintf(printFile,"Wheels' engines were NOT found!\n");
  }

  // Initializing the back engine
  if (ev3_search_tacho_plugged_in(BACK_PORT,0,&engines.backEng,0)) {
    fprintf(printFile,"BACK engine were found!\n");

    set_tacho_stop_action_inx(engines.backEng, TACHO_RESET);
    set_tacho_ramp_up_sp(engines.backEng, 100);  // 0.1 second to reach the full speed
    set_tacho_ramp_down_sp(engines.backEng, 100);// 0.1 is the decceleration time
  } else {
    engines.backEng = DESC_LIMIT;
    fprintf(printFile,"BACK engine were NOT found\n");
  }

  // Initializing the front engine
  if (ev3_search_tacho_plugged_in(FRONT_PORT,0,&engines.frontEng,0)) {
    fprintf(printFile,"FRONT engine were found!\n");

    set_tacho_ramp_up_sp(engines.frontEng, 100); // 0.1 second to reach the full speed
    set_tacho_ramp_down_sp(engines.frontEng, 100); // 0.1 is the decceleration time

    pthread_mutex_init(&turningMutex, NULL);
    if (pthread_create(&movingEyesThread, NULL, move_eyes, NULL) == -1) { // Start the thread using ‘move_eyes’ function
      fprintf(printFile,"pthread_create");
      exit(EXIT_FAILURE);
    }

  } else {
    engines.frontEng = DESC_LIMIT;
    fprintf(printFile,"FRONT engine were NOT found\n");
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
  fprintf(printFile,"Turning by %d° degrees.\n", degree);

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
    fprintf(printFile,"Correcting the current heading %d° by turning %d°.\n",angle,-angle);
    turn(-angle);
  } else if (angle > 45 && angle < (90-ERROR)){
    fprintf(printFile,"Correcting the current heading %d° by turning %d°.\n",angle,90-angle);
    turn(90-angle);
  }

  Sleep(1000);
}

/* END: The wheels' engines functions */

/* BEGIN: The back engine functions */
void backEngine(int direction) {
  if (engines.backEng == DESC_LIMIT) {
    fprintf(printFile,"ERROR: CANNOT USE BACK ENGINE!\n");
    return ;
  }

  set_tacho_speed_sp(engines.backEng, -direction * SPEED_BACK_ENGINE);
  set_tacho_time_sp(engines.backEng, 700);
  set_tacho_command_inx(engines.backEng, TACHO_RUN_TIMED);

  Sleep(2000);
}
/* END: The back engine functions */

/* BEGIN: The front engine functions */
void turnSonar(int angle) {
  if (engines.frontEng == DESC_LIMIT) {
    fprintf(printFile,"ERROR: CANNOT USE FRONT ENGINE!\n");
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
    Sleep(100);
  }
}
/* END: The front engine functions */
