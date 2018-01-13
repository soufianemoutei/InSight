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
#include "client.h"

Position position;

char updating = 1;
pthread_t updateThread;
pthread_mutex_t positionMutex;
State map[MAP_HEIGHT][MAP_WIDTH];

int heading = 90;

void initPosition(float x, float y) {
	position = (Position) {x,y,((int) x / 5),((int) y / 5)};

	printf("INIT: x = %f, y = %f\n", x, y);

	initMap();

	printf("Initial angle: %d \n", heading);

	pthread_mutex_init(&positionMutex, NULL);

	if(pthread_create(&updateThread, NULL, update, NULL) == -1) {
		printf("Failed to create the update thread");
		exit(EXIT_FAILURE);
	}
}

void initMap() {
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			map[i][j] = NOT_VISITED;
		}
	}
}

void getPosition(int16_t* pos) {
	pthread_mutex_lock(&positionMutex);
	pos[0] = position.ux;
	pos[1] = position.uy;
	pthread_mutex_unlock(&positionMutex);
}

void updateMapPosition(int sonarValue, State state) {
	int16_t x = 0, y = 0;
	pthread_mutex_lock(&positionMutex);
	x = (int16_t) ((position.x - (sonarValue / 10) * cos(heading * M_PI / 180)) / 5);
	x = (int16_t) ((position.x + (sonarValue / 10) * sin(heading * M_PI / 180)) / 5);
	map[x][y] = state;
	pthread_mutex_unlock(&positionMutex);
}

// http://www.robotnav.com/position-estimation/
void* update() {
	int angle, newAngle, angleDiff;
	int displacement;
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

		pthread_mutex_lock(&positionMutex);

		heading -=  angleDiff;

		printf("HEADING = %d°\n", heading);

		position.x += displacement * cos(heading * M_PI / 180);
		position.y += displacement * sin(heading * M_PI / 180);
		printf("UPDATING POSITION TO: x = %f, y = %f\n", position.x, position.y);
		if ((((int) (position.x / 5)) != position.ux) || (((int) (position.y / 5)) != position.y)) {
			for (int y = position.uy + 1; y <= ((int) (position.y / 5)); y++) {
				for (int x = position.ux + 1; x <= ((int) (position.x / 5)); x++) {
					if (map[y][x] == NOT_VISITED) {
						printf("UPDATING THE POSITION ON THE MAP: x = %d, y = %d\n", x, y);
						map[y][x] = VISITED;
					}
				}
			}
			position.ux = (int) (position.x / 5);
			position.uy = (int) (position.y / 5);
			printf("SENDING THE POSITION TO THE SERVER: x = %d, y = %d\n", position.ux, position.uy);
			send_position(position.ux,position.uy);
		}
		pthread_mutex_unlock(&positionMutex);

		angle = newAngle;
		leftWheelPrev = leftWheel;
		rightWheelPrev = rightWheel;

		pthread_mutex_lock(&positionMutex);
		if (position.ux < 0 || position.ux > 80 || position.uy < 0 || position.uy > 80) {
			stopRunning();
			exploring = 0;
			break;
		}
		pthread_mutex_unlock(&positionMutex);

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
	printf("Angle given by the gyro sensor: %d\n",getGyroValue());
	//printf("Angle given by the compass sensor: %f\n",getCompassValue());
}
