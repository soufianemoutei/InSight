---
title: "Architecture"
bg: blue
color: white
fa-icon: cogs
style: center
---

# Architecture

Insight’s final architecture was drawn in order to meet several system specifications related to its cartography mission. Therefore, the robot design was continuously changed and improved to cover up each occurring issue. The main core of the robot was inspired by the model given in the user guide manual.

The robot carries 3 sensors and uses 4 motors described as following:

<div class="row display-flex">

<div class="col-xs-12 col-sm-4 col-md-4 col-lg-4">
<div class="feature-icon"><i class="fa fa-flash fa-2x"></i></div>
<div class="feature-title">Sensors</div>
<p class="feature-body">An ultrasonic sensor displaying measures of distance was used to enable obstacles detection including movable/ non-movable obstacles and fences. It was placed at the front bottom of the robot so that it can detect obstacles with a small height dimension such as the ball. The sensor was placed further from the wheels rotation axis to avoid collisions with the wheels. </p>

<p class="feature-body">A light sensor detecting colors enables to distinguish between movable and non-movable obstacles by their color. It was placed at the front so as to discern obstacles after their detection and on top of the ultrasonic sensor in order to avoid bumps.</p> 

<p class="feature-body">A gyroscope was used to measure the angular acceleration and help compute the position of the robot. It was placed at the top of the robot in order to stabilize it and maximize its precision as it’s crucial to get reliable position values. </p>
</div>

<div class="col-xs-12 col-sm-4 col-md-4 col-lg-4">
<div class="feature-icon"><i class="fa fa-heartbeat fa-2x"></i></div>
<div class="feature-title">Motors</div>
<p class="feature-body">Two large motors were used as the driving base of the robot due to their powerful and precise robotic action. 
Another large motor was utilized to carry a relatively heavy platform used to manage releasing and collecting obstacles. This motor choice was based on its power and precision control due to the built-in rotation sensor with 1-degree resolution.</p>

<p class="feature-body">A medium motor was used to enable scanning the area with the ultra sonic sensor. 
The Medium Motor includes a smaller and lighter built-in Rotation Sensor which enables quick responses at an adjustable power level.
</p>
</div>

<div class="col-xs-12 col-sm-4 col-md-4 col-lg-4">
<div class="feature-icon"><i class="fa fa-linux fa-2x"></i>
</div>
<div class="feature-title">Operating System</div>
<p class="feature-body">InSight uses ev3dev, which is a Debian Linux-based operating system that runs on several LEGO® MINDSTORMS compatible platforms including the LEGO® MINDSTORMS EV3 and Raspberry Pi-powered BrickPi.</p>
</div>

</div>

<div class="row display-flex">

<div class="col-xs-12 col-sm-4 col-md-4 col-lg-4">
<div class="feature-icon">
</div>
<div class="feature-title"></div>
<p class="feature-body"></p>
</div>

<div class="col-xs-12 col-sm-4 col-md-4 col-lg-4">
<div class="feature-icon"><i class="fa fa-bluetooth fa-2x"></i>
</div>
<div class="feature-title">Communication</div>
<p class="feature-body">InSight uses Bluetooth to send its work to the server but also to communicate with the other robots.</p>
</div>

All along the construction of the robot, we kept in mind the fulfillment of a symmetric and stable structure by distributing weight equally in order to minimize motion errors such as deviation and rotation errors.

</div>
