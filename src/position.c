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
int heading = 90;

pthread_t updateThread;
pthread_mutex_t positionMutex;
State map[MAP_HEIGHT][MAP_WIDTH];

void initPosition(float x, float y) {
	position = (Position) {x,y,((int) x / 5),((int) y / 5)};

	printf("Initializing the position: x = %f, y = %f.\n", x, y);
	printf("Initial heading: %d.\n", heading);

	initMap();

	pthread_mutex_init(&positionMutex, NULL);

	if(pthread_create(&updateThread, NULL, update, NULL) == -1) {
		printf("Failed to create the update thread");
		exit(EXIT_FAILURE);
	}
}

void initMap() {
	printf("Initializing the map.\n"); // With NOT_VISITED
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

void updateMapPosition(int sonarValue) {
	int16_t x = 0, y = 0;
	pthread_mutex_lock(&positionMutex);
	x = (int16_t) ((position.x + (sonarValue / 10) * cos(heading * M_PI / 180)) / 5); // Work out the position of the obstacle using the current position of the robot and the value given by the sonar sensor
	y = (int16_t) ((position.y + (sonarValue / 10) * sin(heading * M_PI / 180)) / 5);
	map[y][x] = OBSTACLE;
	pthread_mutex_unlock(&positionMutex);
}

//Based on: http://www.robotnav.com/position-estimation/
void* update() {
	int angle, newAngle, angleDiff;
	int displacement;
	int leftWheel = 0, rightWheel = 0, leftWheelPrev = 0, rightWheelPrev = 0; // Used to store the values returned by the wheels

	angle = getGyroValue();
	printf("Angle given by the gyro sensor: %d.\n",angle);

	leftWheelPrev = leftWheelPosition();
	rightWheelPrev = rightWheelPosition();

	while (updating) {
		if (!isRunning()) {
			continue;
		}
		newAngle = getGyroValue();
		printf("Angle given by the gyro sensor: %d.\n", newAngle);

		leftWheel = leftWheelPosition();
		rightWheel = rightWheelPosition();

		displacement = (leftWheel + rightWheel - leftWheelPrev - rightWheelPrev) * (5.6 * M_PI / 360.0) / 2.0; // Work out the wheels displacement
		angleDiff = newAngle - angle; // Work out the relative rotation of the robot

		pthread_mutex_lock(&positionMutex);
		heading -=  angleDiff; // Updating the heading

		printf("Updating the heading to %d°.\n", heading);

		position.x += displacement * cos(heading * M_PI / 180); // Trigonometric relations; to update the position
		position.y += displacement * sin(heading * M_PI / 180);
		printf("Updating the position to: (x = %f, y = %f).\n", position.x, position.y);
		if ((((int) (position.x / 5)) != position.ux) || (((int) (position.y / 5)) != position.y)) { // Check if the robot changes the square (on the map)
			for (int y = position.uy + 1; y <= ((int) (position.y / 5)); y++) {
				for (int x = position.ux + 1; x <= ((int) (position.x / 5)); x++) {
					if (map[y][x] == NOT_VISITED) { // Update the position on the map only if it's never visited
					printf("Updating the position on the map to: (x = %d, y = %d).\n", x, y);
					map[y][x] = VISITED;
					}
				}
			}
		position.ux = (int) (position.x / 5);
		position.uy = (int) (position.y / 5);
		send_position(position.ux,position.uy); // Send the position to the server
		}
	pthread_mutex_unlock(&positionMutex);

	angle = newAngle;
	leftWheelPrev = leftWheel;
	rightWheelPrev = rightWheel;
	}

}

void freePosition() {
	updating = 0;
	if (pthread_join(updateThread, NULL)) {
		perror("Failed to close the update thread");
		exit(EXIT_FAILURE);
	}
}
