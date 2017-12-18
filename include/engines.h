#include "ev3.h"

#ifndef __ENGINES__
#define __ENGINES__

#define Sleep( msec ) usleep(( msec ) * 1000 )

#define LEFT_PORT 68
#define RIGHT_PORT 65
#define BACK_PORT 66
#define FRONT_PORT 67
#define MAXSPEED 1050
#define SPEED MAXSPEED/8

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
extern int runningDirection;

void initEng();
void goStraight(int time);
void turn(int degree);
void stopRunning();
int isRunning();
int leftWheelPosition();
int rightWheelPosition();
void explore();
void backEngine(char direction);
void exploreSmallArena();
void goToNextCheckpoint(int currentCpId);
void changeLayer();
void turnSonar(int angle);

#endif
