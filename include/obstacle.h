#ifndef __OBSTACLE__
#define __OBSTACLE__

#define DISTANCE_FROM_OBSTACLE 350

char closeToObstacles();
char farObstacle();
void detectObstacles();
void actionAfterDetecting();


#endif

robot@ev3dev:~/ev3dev-c/source/ev3/InSight$ cat include/obstacle.h 
#ifndef __OBSTACLE__
#define __OBSTACLE__

#define DISTANCE_FROM_OBSTACLE 350

char closeToObstacles();
char farObstacle();
void detectObstacles();
void actionAfterDetecting();


#endif
