#include <pthread.h>

#ifndef __POSITION__
#define __POSITION__

#define UPDATE_TIME 2

typedef struct Position {
   float x;
   float y;
} Position;

extern Position position;
extern int updating;
extern pthread_t updateThread;
extern pthread_mutex_t positionMutex;

void initPosition(float x, float y);

void getPosition(float* x, float* y);

void* update();

void freePosition();

char nextToWall();

void getAngleFromSensors();

#endif
