#include <pthread.h>
#include "ev3.h"

#ifndef __POSITION__
#define __POSITION__

#define UPDATE_TIME 2
#define MAP_HEIGHT 80
#define MAP_WIDTH 80

typedef struct Position {
   float x;
   float y;
   int16_t ux;
   int16_t uy;
} Position;

typedef enum State {
  NOT_VISITED = 0,
  VISITED = 1,
  OBSTACLE = 2
} State;

extern Position position;
extern char updating;
extern pthread_t updateThread;
extern pthread_mutex_t positionMutex;
extern State map[MAP_HEIGHT][MAP_WIDTH];

void initPosition(float x, float y);
void initMap();
void getPosition(int16_t* pos);
void updateMapPosition(int sonarValue, State state);
void* update();
void freePosition();
void getAngleFromSensors();

#endif
