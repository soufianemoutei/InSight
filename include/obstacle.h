#ifndef __OBSTACLE__
#define __OBSTACLE__

#define DISTANCE_FROM_OBSTACLE 200 // The minimum distance between the robot and the obstacle to avoid bumping into it.

char closeToObstacles(); // Check if the robot is close to an obstacle.
char isBall(); // Check if the obstacle is a ball (movable obstacle).

void BasicReaction(); // A strategy to avoid an obstacle by going to the more vacant direction.

/* A strategy to avoid an obstacle by going to its other side. Hence, the function name. */
void fourTurnsAction(int calls); // The function performs four turns to go to the other side of the obstacle.
void checkBeforeTurn(); // Check the robot's left if it contains an obstacle.

#endif
