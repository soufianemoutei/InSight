#include <pthread.h>
#include "ev3.h"

#ifndef __NAVIGATION__
#define __NAVIGATION__

extern char exploring; // If (exploring = 1): the robot will start exploring the map until (exploring = 0).

void explore(); // Explore the arena: the procedure
void snake(); // A ‘snake’ strategy to explore the arena
void strategy_beta(); // Another strategy to explore the arena. -- Not very efficient due to the randomness --
void exploreUsingLayers(); // Another strategy to explore the arena by considering it as multiple layers
void exploreLayer(int currentLayerID); // Explore the layer ‘currentLayerID’

#endif
