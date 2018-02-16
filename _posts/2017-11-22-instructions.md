---
title: "Instructions"
bg: '#DCBB7D'
color: white
fa-icon: terminal
---

# **ALGORITHMS**

## CARTOGRAPHY STRATEGY & OBSTACLES HANDLING

We decided to adopt a snake movement to explore the arenas. This method consists of scanning the area at the first round in horizontal lines and at the second round in vertical lines. The second exploration enables to visit places that were not explored at the first round due to the detection of an obstacle in the path. When the robot completes its first round (i.e. it’s blocked by three fences and follows the same path again) it turns by 90 degrees and repeats the same maneuver vertically.

Whenever there is an obstacle detected in the range of 20 centimeters by the front sensor, the robot will stop, move backward for few milliseconds to ensure it has enough space and take a turn in either the right or left direction depending on its current position. The choice of 20 centimeters was made after several tests considering the speed and acceleration of the robot.

### Small arena vs Big arena:

Two strategies are followed depending on the type of the arena. Contrary to the small arena that has four real wooden fences, the big arena may have a virtual starting fence. To inform the robot of the existence of the virtual fence we use the robot coordinates as its starting point is located at the virtual fence. Consequently, when the position of the robot gets negative, it means that it has reached the limit of the area to scan. This affirmation is used as condition to put the robot on a new vertical line after going backward for 1.5 seconds.

Acceleration and speed parameters are set in order to enable a progressive stop of the robot.

{% highlight clike linenos=table %}
while (time_spent < 200s AND server is still allowing the robot to continue the exploration) {
  Correct the heading every two iterations.
  angle = getGyroValue();
  Run until InSight sees an obstacle or if it's out of the arena;
  error = getGyroValue() - angle;
  Turn the robot by -error° degrees if error is larger than ERROR;
  Stop turning the sonar sensor in the thread;

  if (closeToObstacles() && !isBall()) {
    Add the obstacle to the map if it isn't movable;
  }

  if (!onTheMap()) {
    Run backward for 1.5 seconds to return to the arena;
  } else {
    Run backward for some milliseconds before turning to an another side;
  }

  Quarter-turn to avoid the obstacle OR to avoid leaving again the arena;
  If there are obstacles to release, release one and inform the server that the robot has just released an obstacle;

  if (!closeToObstacles()) {
    Run for 0.85 seconds to go to the next line in order to explore it;
  } else {
    Add the obstacle to the map if it isn't movable;
  }

  Quarter-turn to explore the new line;
  If the limit of the consecutive obstacles is reached (2), quarter-turn to get out of the corner;
  Continue turning the sonar sensor in the thread;
}
{% endhighlight %}

### Correction/ Calibration of the robot to increase its accuracy:
Since many factors influence the motion stability of the robot, we decided to add a function that stabilizes the robot if it deviates from its original path by using the value of the gyroscope. Indeed, if the difference between the current and original value is more than 15 degrees the robot has to stop and return to its original path.

{% highlight clike linenos=table %}
angle = getHeading() mod 90;
if (angle > ERROR AND angle <= 45) {
  Turn by -angle° degrees;
} else if (angle > 45 AND angle < (90-ERROR)){
  Turn by (90-angle)° degrees;
}
{% endhighlight %}

### Thread for moving-eyes:
To avoid bumping into obstacles located at the front right or left of the robot, a new thread was implemented to rotate periodically the ultrasonic sensor located at the front of the robot. The window angle was set to approximately 40 degrees in order to avoid bumping into obstacles located at the front of the robot while moving its eyes.

{% highlight clike linenos=table %}
direction = 1;
ANGLE_THREAD_SONAR = 40;
while (exploring) {
  Turn the sonar sensor by (direction * ANGLE_THREAD_SONAR)° degrees;
  Turn the sonar sensor by -(direction * ANGLE_THREAD_SONAR)° degrees;
  direction = -direction;
}
{% endhighlight %}

### Other strategies dropped:

1 - Exploration following layers:
The robot explores the arena by repeating recursively the same algorithm on different layers of the arena until it has visited all the squares of the map. The first layer being the whole perimeter of the arena and the second being the new perimeter after excluding the first layer. The drawback of this method was to steer clear of the obstacles that required rotating the front sonar in their direction which caused bumping into front obstacles when the sensor was not facing them.

2 - Exploration by avoiding collisions:
This simple method enables to detect obstacles and correspondingly change the direction of the robot to the one free from obstacles (i.e. the one that has the highest value of the sonar) to avoid collisions. Therefore, when the front sensor detects obstacles within the range of 20 centimeters, the robot is made to turn right or left. If both the right and left side are free from obstacles, the robot would have to turn to the side that was less visited. The drawback of the method was its randomness that might lead to a limited exploration of the arena.


## POSITION TRACKING

The estimated position of the robot is determined by combining the values of the optical encoders used to measure wheel rotation and the robot's heading with the embedded gyroscope.
With this known parameters, the robot can estimate at any time of the exploration its current position using the following mathematical equations:

- To work out the wheel displacement:
{% highlight clike linenos=table %}
WHEEL_DIAMETER = 5.6;
COUNTS_PER_REVOLUTION = 360;
ENCODER_SCALE_FACTOR = PI * WHEEL_DIAMETER / COUNTS_PER_REVOLUTION;

DISPLACEMENT = (LEFT_ENCODER_COUNT + RIGHT_ENCODER_COUNT) * ENCODER_SCALE_FACTOR / 2;
{% endhighlight %}

- To work out the robot heading: (initial heading is 90°)
{% highlight clike linenos=table %}
HEADING = HEADING - (getGyroValue() - getGyroPreviousValue());
{% endhighlight %}

- To work out the robot position:
{% highlight clike linenos=table %}
POS_X = POS_X + DISPLACEMENT * cos(HEADING)
POS_Y = POS_Y + DISPLACEMENT * sin(HEADING)
{% endhighlight %}

We used a C structure to represent the robot position:

{% highlight clike linenos=table %}
typedef struct Position {
   float x; // The x-position
   float y; // The y-position
   int16_t ux; // The x-position of the map, it equals x-position * 5 (each square on the map has sides with 5cm long)
   int16_t uy; // The y-position of the map, it equals y-position * 5 (each square on the map has sides with 5cm long)
   int heading; // The current heading of the robot. The initial value is 90°
} Position;
{% endhighlight %}

The robot position is updated in a thread. The coordinates x and y are updated while the robot is still exploring. The coordinates ux and uy are updated, only if the robot changes the square, by working out respectively the nearest integer to the real numbers x/5 and y/5.

{% highlight clike linenos=table %}
POS_UX = getNearestInteger(POS_X / 5);
POS_UX = getNearestInteger(POS_Y / 5);
{% endhighlight %}

* This method was implemented based on the known Gyroscope Enhanced Odometry approach presented at the following website: http://www.robotnav.com/position-estimation/

## Generating the map

We use a MAP_HEIGHT * MAP_WIDTH matrix of an enumeration representing the state of the squares (each square has sides with 5cm long):

{% highlight clike linenos=table %}
typedef enum State {
  NOT_VISITED = 0, // Unknown state
  EMPTY = 1, // Empty, the robot can visit it
  OBSTACLE = 2 // Non-movable obstacle
} State;
{% endhighlight %}

First, the map is initialized with NOT_VISITED values. Afterwards, in the thread updating the position, if the robot visits a new square that is NOT_VISITED, its value will be changed to EMPTY.

If an obstacle is detected by the sonar sensor, the latter will send the value sonarValue representing the distance (in millimeters) between the robot and the obstacle. First we check if the obstacle is movable or not by checking its color (a movable obstacle is always a RED ball of diameter 3.5cm) and the color on the left and on the right of the obstacle (to make sure the obstacle isn't a red non-movable obstacle whose length has to be larger than 5cm); the color is detected by the light sonar:

{% highlight clike linenos=table %}
function isBall() {
  color = getColorValue(); // If it equals 5, the color is red.

  Turn the sonar sensor by 20° degrees to the right;
  colorRight = getColorValue();

  Turn the sonar sensor by 40° degrees to the right;
  colorLeft = getColorValue();

  Turn the sonar sensor by 20° degrees to the left;

  return (color == 5 AND colorRight != 5 AND colorLeft != 5);
}
{% endhighlight %}


When we make sure the obstacle in non-movable, we update the square value with OBSTACLE. To do so, we work out the position of the obstacle using the position of the robot and the its heading:

{% highlight clike linenos=table %}
POS_X_OBSTACLE = POS_X + (sonarValue / 10) * cos(HEADING);
POS_Y_OBSTACLE = POS_Y + (sonarValue / 10) * sin(HEADING);
{% endhighlight %}

Then, we update the map:

{% highlight clike linenos=table %}
POS_UX_OBSTACLE = getNearestInteger(POS_X_OBSTACLE / 5);
POS_UY_OBSTACLE = getNearestInteger(POS_X_OBSTACLE / 5);

map[POS_UY_OBSTACLE][POS_UX_OBSTACLE] = OBSTACLE;
{% endhighlight %}

After the exploration, we correct the map in 4 rounds:

1 - We correct the non-visited squares on the map by voting (we look at the squares around).

{% highlight clike linenos=table %}
numberOfEmptySquaresAround = 0;
numberOfObstaclesAround = 0;
numberOfNonVisistedAround = 0;

for y = 0,...,MAP_HEIGHT-1 {
  for x = 0,...,MAP_WIDTH-1 {
    if (map[y][x] == NOT_VISITED) {
      We look at the squares (x,y-1) (if y >= 1), (x,y+1) (if y < MAP_HEIGHT-1), (x-1,y) (if x >= 1) and (x+1,y) (if x < MAP_WIDTH-1);
      numberOfEmptySquaresAround = the number of empty squares within the 4 squares;
      numberOfObstaclesAround = the number of obstacle squares within the 4 squares;
      numberOfNonVisistedAround = the number of non-visited squares within the 4 squares;

      If numberOfObstaclesAround is larger than numberOfEmptySquaresAround and numberOfNonVisistedAround, change (x,y) to an obstacle:
      map[y][x] = OBSTACLE;

      If numberOfEmptySquaresAround is larger than or equals numberOfObstaclesAround and numberOfNonVisistedAround, change (x,y) to empty:
      map[y][x] = EMPTY;
    }
    numberOfEmptySquaresAround = 0;
    numberOfObstaclesAround = 0;
    numberOfNonVisistedAround = 0;
  }
}
{% endhighlight %}

2 - We correct the empty squares by eliminating those surrounded by three obstacles.

{% highlight clike linenos=table %}
numberOfObstaclesAround = 0;

for y = 0,...,MAP_HEIGHT-1 {
  for x = 0,...,MAP_WIDTH-1 {
    if (map[y][x] == EMPTY) {
      We look at the squares (x,y-1) (if y >= 1), (x,y+1) (if y < MAP_HEIGHT-1), (x-1,y) (if x >= 1) and (x+1,y) (if x < MAP_WIDTH-1);
      numberOfObstaclesAround = the number of obstacle squares within the 4 squares;

      If numberOfObstaclesAround is larger than or equals three, change (x,y) to an obstacle:
      map[y][x] = OBSTACLE;
    }
    numberOfObstaclesAround = 0;
  }
}
{% endhighlight %}

3 - We correct again the non-visited squares on the map by voting.


4 - We substitute non-visited squares by empty squares.

{% highlight clike linenos=table %}
for y = 0,...,MAP_HEIGHT-1 {
  for x = 0,...,MAP_WIDTH-1 {
    if (map[y][x] == NOT_VISITED) {
      map[y][x] = EMPTY;
    }
  }
}
{% endhighlight %}

After the correcting, the map is ready and we send it to the server: the obstacles are represented with red squares; the empty squares are white.


## COMPETITION WITH OTHER ROBOTS
The strategy adopted to compete with other robots was to release an obstacle in the front of the first non-movable obstacle detected so as to mislead other teams about the identity of the obstacle.
The robot approaches the non-movable obstacle turns to its right or left, releases the ball then continues its trajectory.

# **USING THE SOURCE CODE**

## PREREQUISITES

- Please follow the detail instruction on EV3 official page for installing the library, here: http://www.ev3dev.org/docs/getting-started/

- Connect to the EV3 robot by ssh. If you are connection via USB, please follow this instruction: http://www.ev3dev.org/docs/tutorials/connecting-to-the-internet-via-usb/

## DOWNLOAD

After connecting to the EV3, execute in the command line:

{% highlight bash linenos=table %}
git clone --recursive https://github.com/soufianemoutei/InSight
{% endhighlight %}

## COMPILE

{% highlight bash linenos=table %}
cd InSight
make
{% endhighlight %}

## RUN

{% highlight bash linenos=table %}
./InSight
{% endhighlight %}
