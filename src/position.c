#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include "position.h"
#include "engines.h"
#include "sensors.h"
#include "ev3.h"
#include "ev3_sensor.h"
#include "ev3_port.h"

Position position;

int updating = 1;
pthread_t updateThread;
pthread_mutex_t positionMutex;

int heading = 90;

void initPosition(float x, float y) {
	position = (Position) {x,y};

	printf("INIT: x = %f, y = %f\n", x, y);
	//heading = getGyroValue();
	printf("Initial angle: %d \n", heading);

	pthread_mutex_init(&positionMutex, NULL);

	if(pthread_create(&updateThread, NULL, update, NULL) == -1) {
		printf("Failed to create the update thread");
		exit(EXIT_FAILURE);
	}
}

void getPosition(float* x, float* y) {
	pthread_mutex_lock(&positionMutex);
	*x = position.x;
	*y = position.y;
	pthread_mutex_unlock(&positionMutex);
}

void* update() {
	int angle, newAngle, angleDiff;
	int displacement; // http://www.robotnav.com/position-estimation/
	int leftWheel = 0, rightWheel = 0, leftWheelPrev = 0, rightWheelPrev = 0;

	angle = getGyroValue();

	leftWheelPrev = leftWheelPosition();
	rightWheelPrev = rightWheelPosition();

	while (updating) {
		if (!isRunning()) {
			continue;
		}
		newAngle = getGyroValue();

		getAngleFromSensors();

		leftWheel = leftWheelPosition();
		rightWheel = rightWheelPosition();

		displacement = (leftWheel + rightWheel - leftWheelPrev - rightWheelPrev) * (5.6 * M_PI / 360.0) / 2.0;
		angleDiff = newAngle - angle;

		heading -=  angleDiff;

		printf("HEADING = %d°\n", heading);

		pthread_mutex_lock(&positionMutex);
		position.x -= displacement * cos(heading * M_PI / 180);
		position.y += displacement * sin(heading * M_PI / 180);
		printf("UPDATING POSITION TO: x = %f, y = %f\n", position.x, position.y);
		pthread_mutex_unlock(&positionMutex);

		angle = newAngle;
		leftWheelPrev = leftWheel;
		rightWheelPrev = rightWheel;

		pthread_mutex_lock(&positionMutex);
		if (position.x < 0.0 || position.x > 120.0 || position.y < 0.0 || position.y > 200.0) {
			stopRunning();
			break;
		}
		pthread_mutex_unlock(&positionMutex);

		printf("NEXT TO WALL:%d\n", nextToWall());

		sleep(UPDATE_TIME);
	}

	pthread_exit(NULL);
}

char nextToWall() {
	char near = 0;
	pthread_mutex_lock(&positionMutex);
	if (position.x < 1.5 || position.x > 100.0) {
		near = 1;
	}
	if (position.y < 1.5 || position.y > 190.0) {
		near = 1;
	}
	pthread_mutex_unlock(&positionMutex);
	return near;
}

void freePosition() {
	updating = 0;
	if (pthread_join(updateThread, NULL)) {
		perror("Failed to close the update thread");
		exit(EXIT_FAILURE);
	}
}

void getAngleFromSensors() {
	printf("Angle given by the gyro sensor: %d\n",getGyroValue());
	//	printf("Angle given by the compass sensor: %f\n",getCompassValue());
}
