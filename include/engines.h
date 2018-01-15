#include <pthread.h>
#include "ev3.h"

#ifndef __ENGINES__
#define __ENGINES__

#define DEGREE_TO_COUNT( d )  (( d ) * 190 / 90) // Work out the relative position from the angle ‘d’°.
#define DEGREE_TO_TIME_FE( d )  (( d ) * 1000 / 180 ) // Work out the time required to turn the sonar sensor by ‘d’° degrees.

#define LEFT_PORT 68 // The left wheel's engine port
#define RIGHT_PORT 65 // The right wheel's engine port
#define BACK_PORT 66 // The back engine port. This engine is used to release an obstacle.
#define FRONT_PORT 67 // The front engine port. This engine is used to turn the sonar sensor.

#define MAXSPEED 1050 // The maximum speed of the wheels
#define SPEED_LINEAR MAXSPEED/2 // The wheels' engines speed for linear motion
#define SPEED_CIRCULAR MAXSPEED/4 // The wheels' engines speed for circular motion
#define SPEED_FRONT_ENGINE MAXSPEED/5 // The front engine speed
#define SPEED_BACK_ENGINE MAXSPEED/6 // The back engine speed

#define ERROR 15 // The threshold to correct the error accumulated during the rotations of the robot

#define ANGLE_THREAD_SONAR 40 // The absolute value of the angle used to turn the sonar sensor in the thread.

typedef struct Wheel {
  uint8_t left; // Left wheel's engine
  uint8_t right; // Right wheel's engine
  uint8_t both[3]; // An array to control the two engines at the same time. The third element is always DESC_LIMIT which is the limit of tacho descriptors.
} Wheel;

typedef struct Engines {
  Wheel wheelEng; // Wheels' engines
  uint8_t backEng; // Back engine
  uint8_t frontEng; // Front engine
} Engines;

extern Engines engines; // The variable containing the four engines.
extern char exploring; // If (exploring = 1): the robot will start exploring the map until (exploring = 0).
extern int directionOfSonarSensor; // The sonar sensor will turn in the trigonemtric direction if (directionOfSonarSensor = 1), in the clockwise direction if (directionOfSonarSensor = -1)
extern int obstaclesToRelease; // The number of obstacles to release

extern pthread_t movingEyesThread; // A thread to move the eyes (Sonar sensor) of the robot.
extern pthread_mutex_t turningMutex; // To control the thread of the sonar sensor: If an obstacle is found, the robot will stop moving its eyes.

void initEng(); // Initializing the engines

/*
If (time > 0), the robot goes straight for ‘time’ milliseconds.
If (time = 0), the robot will run for an infinite time until it receives the TACHO_STOP command.
The robot goes forward if (direction = 1), backward if (direction = -1).
*/
void goStraight(int time, int direction);

void turn(int degree); // Turn ‘degree’° degrees.
void stopRunning(); // Stop running by sending TACHO_STOP command.
char isRunning(); // Returns 1 if the two wheels are still running, 0 otherwise.
int leftWheelPosition(); // Return the position attribute of the left wheel's engine.
int rightWheelPosition(); // Return the position attribute of the right wheel's engine.
void correctHeading(); // Correct the heading if it and (k * 90 , k is an integer) aren't close by ERROR.
void backEngine(int direction); // Ask the engine to go up if (direction = 1), down if (direction = -1).
void turnSonar(int angle); // Turn the sonar engine by ‘angle’ degrees.
void* move_eyes(); // The function the thread runs to turn the sonar sensor to the left or to the right while the robot is exploring the map.
void explore(); // Explore the arena: the procedure
void snake(); // A ‘snake’ strategy to explore the arena
void strategy_beta(); // Another strategy to explore the arena. -- Not very efficient due to the randomness --
void exploreUsingLayers(); // Another strategy to explore the arena by considering it as multiple layers
void exploreLayer(int currentLayerID); // Explore the layer ‘currentLayerID’

#endif
