#include <pthread.h>
#include "ev3.h"

#ifndef __ENGINES__
#define __ENGINES__

#define Sleep( msec ) usleep(( msec ) * 1000 )
#define DEGREE_TO_COUNT( d )  (( d ) * 190 / 90 )
#define DEGREE_TO_TIME_FE( d )  (( d ) * 1000 / 180  )

#define LEFT_PORT 68
#define RIGHT_PORT 65
#define BACK_PORT 66
#define FRONT_PORT 67
#define MAXSPEED 1050
#define SPEED_LINEAR MAXSPEED/2
#define SPEED_CIRCULAR MAXSPEED/5
#define SPEED_FRONT_ENGINE MAXSPEED/5
#define SPEED_BACK_ENGINE MAXSPEED/6

#define ERROR 15

typedef struct Wheel {
  uint8_t left;
  uint8_t right;
  uint8_t both[3];
} Wheel;

typedef struct Engines {
  Wheel wheelEng;
  uint8_t backEng;
  uint8_t frontEng;
} Engines;

extern Engines engines;
extern char exploring;

extern pthread_mutex_t turningMutex;
extern pthread_t movingEyesThread;

void initEng();
void goStraight(int time, int direction);
void turn(int degree);
void stopRunning();
int isRunning();
int leftWheelPosition();
int rightWheelPosition();
void correctHeading();
void backEngine(int direction);
void explore();
void turnSonar(int angle);
void snake();
void* move_eyes();

#endif
