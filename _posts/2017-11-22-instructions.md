---
title: "Instructions"
bg: '#DCBB7D'
color: white
fa-icon: terminal
---

# **ALGORITHMS**

## CARTOGRAPHY STRATEGY & OBSTACLES HANDLING

We decided to adopt a snake movement to explore the arenas. This method consists of scanning the area at the first round in horizontal lines and at the second round in vertical lines. The second exploration enables to visit places that were not explored at the first round due to the detection of an obstacle in the path. When the robot completes its first round (i.e. it’s blocked by three fences and follows the same path again) it turns by 90 degrees and repeats the same maneuver vertically.

Whenever there is an obstacle detected in the range of 20 centimeters by the front sensor, the robot will stop, move backward for few milliseconds to ensure it has enough space and take a turn in either the right or left direction depending on its current position. The choice of 20 centimeters was made after several tests considering the speed and acceleration of the robot since it doesn’t stop immediately after the detection.

Small arena vs Big arena:

Two strategies are followed depending on the type of the arena. Contrary to the small arena that has four real wooden fences, the big arena may have a virtual starting fence. To inform the robot of the existence of the virtual fence we use the robot coordinates as its starting point is located at the virtual fence. Consequently, when the position of the robot gets negative it means that it has reached the limit of the area to scan. This affirmation is used as condition to put the robot on a new vetical line.

Acceleration and speed parameters are set in order to enable a progressive stop of the robot. 

Correction/ Calibration of the robot to increase its accuracy:
Since many factors influence the motion stability of the robot, we decided to add a function that stabilizes the robot if it deviates from its original path by using the value of the gyroscope. Indeed, if the difference between the current and original value is more than 15 degrees the robot has to stop and return to its original path.

Thread for moving-eyes:
To avoid bumping into obstacles located at the front right or left of the robot, a new thread was implemented to rotate periodically the ultrasonic sensor located at the front of the robot. The window angle was set to a small value (approximately 30 degrees) in order to avoid bumping into obstacles located at the front of the robot while moving its eyes.


Other strategies dropped:
1/Exploration following layers: 
The robot explores the arena by repeating recursively the same algorithm on different layers of the arena until it has visited all the squares of the map. The first layer being the whole perimeter of the arena and the second being the new perimeter after excluding the first layer. The drawback of this method was to steer clear of the obstacles that required rotating the front sonar in their direction which caused bumping into front obstacles when the sensor was not facing them.

2/Exploration by avoiding collisions:
This simple method enables to detect obstacles and correspondingly change the direction of the robot to the one free from obstacles (i.e. the one that has the highest value of the sonar) to avoid collisions. Therefore, when the front sensor detects obstacles within the range of 20 centimeters, the robot is made to turn right or left. If both the right and left side are free from obstacles, the robot would have to turn to the side that was less visited. The drawback of the method was its randomness that might lead to a limited exploration of the arena.


## POSITION TRACKING

The estimated position of the robot is determined by combining the values of the rotational speed of the wheels with a tachometer and the robot’s  heading with the embedded gyroscope.
With this known parameters, the robot can estimate at any time of the exploration its current position using the following mathematical equations:

{% highlight clike linenos=table %}
POS_X = POS_X + DISPLACEMENT * cos(HEADING + ROTATION / 2)
POS_Y = POS_Y + DISPLACEMENT * sin(HEADING + ROTATION / 2)
{% endhighlight %}

*This method was implemented based on the known Odometry method presented at the following website: http://www.robotnav.com/position-estimation/


## COMPETITION WITH OTHER ROBOTS
The strategy adopted to compete with other robots was to release a movable obstacle (red ball) in the front of the first non-movable obstacle detected so as to mislead other teams about the identity of the obstacle.
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
