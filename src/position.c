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

void initPosition(float x, float y) {
	position = (Position) {x,y};

	printf("INIT: x = %f, y = %f\n", x, y);

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
	float angle, newAngle, angleDiff;
	int displacement; // http://www.robotnav.com/position-estimation/
	int leftWheel = 0, rightWheel = 0;

	angle = getGyroValue();

	while (updating) {
		newAngle = getGyroValue();
		
		getAngleFromSensors();

		leftWheel = leftWheelPosition() - leftWheel;
		rightWheel = rightWheelPosition() - rightWheel;

		displacement = (rightWheel + leftWheel) * (5.6 * M_PI / 360.0) / 2.0;
		angleDiff = newAngle - angle;

		if (angleDiff < 0.0) {
			angleDiff += 360.0;
		}

		if (angleDiff > 360.0) {
			angleDiff -= 360.0;
		}

		pthread_mutex_lock(&positionMutex);
		position.x += displacement * cos(angle * M_PI / 180.0);
		position.y += displacement * sin(angle * M_PI / 180.0);
		printf("UPDATING POSITION TO: x = %f, y = %f\n", position.x, position.y);
		pthread_mutex_unlock(&positionMutex);

		angle = newAngle;

		sleep(UPDATE_TIME);
	}

	pthread_exit(NULL);
}

void freePosition() {
	updating = 0;
	if (pthread_join(updateThread, NULL)) {
		perror("Failed to close the update thread");
		exit(EXIT_FAILURE);
	}
}

void getAngleFromSensors() {
	printf("Angle given by the gyro sensor: %f\n",getGyroValue());
	printf("Angle given by the compass sensor: %f\n",getCompassValue());
}
