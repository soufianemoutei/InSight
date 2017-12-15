#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_sensor.h"
#include "ev3_tacho.h"
#include "engines.h"
#include "sensors.h"
#include "obstacle.h"

 
void detectObstacles() {
  while (isRunning() && getSonarValue() > DISTANCE_FROM_OBSTACLE);
  if (getSonarValue() <= DISTANCE_FROM_OBSTACLE) {
    stopRunning();
    printf("An obstacle was found! The distance from this obstacle is: %dmm.\n", getSonarValue());
    //if (getColorValue() == 5) { // BALL
    if (0) { // BALL
      printf("MOVABLE Obstacle!");
      upAction();
      sleep(2);
      goStraight(2000);
      sleep(2);
      downAction();
      sleep(2);
      turn(runningDirection*90);
      sleep(2);
      upAction();
      sleep(2);
      turn(-runningDirection*90);
      sleep(2);
      downAction();
      sleep(2);
      goStraight(0);
      detectObstacles();
    } else {
      printf("NON-MOVABLE Obstacle!");
      turn(-runningDirection*90);
      sleep(4);
      goStraight(3000);
      sleep(4);
      turn(-runningDirection*90);
      goStraight(3000);
    }

  }
}

/*
void move_Obstacle(int position){
if (!inCenter)
// compute center - go to center
// catch at right distance
// move
// release in position ?
}

/*int main{
for (;;){


goStraight(10000);
value=get_distance();

if (distance<200){
val=get_color();
//change speed of motors or stop
set_tacho_command_inx(wheelEng.right, TACHO_STOP);
set_tacho_command_inx(wheelEng.left, TACHO_STOP);
turn(70)
if (val == "RED")printf("movable obstacle detected at \r(%f) \n ",value)

else printf("non-movable obstacle detected at \r(%f) \n ",value)
}
return 0;
}
}
*/
