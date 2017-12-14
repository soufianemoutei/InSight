#include "ev3.h"

#ifndef __ENGINES__
#define __ENGINES__

#define Sleep( msec ) usleep(( msec ) * 1000 )

#define LEFT_PORT 68
#define RIGHT_PORT 65
#define UP_PORT 66
#define MAXSPEED 1050
#define SPEED MAXSPEED/8

typedef struct Wheel {
  uint8_t left;
  uint8_t right;
  uint8_t both[2];
} Wheel;

typedef struct Engines {
  Wheel wheelEng;
  uint8_t upEng;
} Engines;

extern Engines engines;
extern int runningDirection;

void initEng();
void goStraight(int time);
void turn(int degree);
void stopRunning();
int isRunning();
int leftWheelPosition();
int rightWheelPosition();
void explore();
void upAction();
void downAction();

#endif
