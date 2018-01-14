#include <pthread.h>
#include "ev3.h"

#ifndef __POSITION__
#define __POSITION__

#define MAP_HEIGHT 80 // The map height
#define MAP_WIDTH 80 // The map width
#define STARTING_POSITION_X 8 // The X of the starting position
#define STARTING_POSITION_Y 2 // The Y of the starting position

typedef struct Position {
   float x; // The x-position
   float y; // The y-position
   int16_t ux; // The x-position of the map, it equals x-position * 5 (each square on the map has sides with 5cm long)
   int16_t uy; // The y-position of the map, it equals y-position * 5 (each square on the map has sides with 5cm long)
} Position;

typedef enum State {
  NOT_VISITED = 0,
  VISITED = 1,
  OBSTACLE = 2
} State;

extern Position position; // The current position of the robot
extern int heading; // The current heading of the robot. The initial value is 90°
extern char updating; // We update the position while updating = 1
extern pthread_t updateThread; // For the thread updating the position
extern pthread_mutex_t positionMutex; // To control the updating on the variables position and map
extern State map[MAP_HEIGHT][MAP_WIDTH]; // The map of the arena. It's a matrix whose elements are States: NOT_VISITED, VISITED (but there were no non-movable obstacles), OBSTACLE (non-movable obstacles)

void initPosition(float x, float y); // Init the position, the map and the threads
void initMap(); // Init the map with NOT_VISITED
void getPosition(int16_t* pos); // Update pos with the current position coordinates
void updateMapPosition(int sonarValue); // It's called when a non-movable obstacle was found, it updates the position of the obstacle on the map with the value OBSTACLE
void* update(); // The function used with the updating thread. It updates the position and the map
void freePosition(); // It stops updating and the updating thread as well

#endif
