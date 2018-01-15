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
	position = (Position) {x,y,getNearestInteger(x / 5),getNearestInteger(y / 5)};

	printf("Initializing the position: x = %f, y = %f, x-MAP = %d, y-MAP = %d.\n", x, y, position.ux, position.uy);
	printf("Initial heading: %d.\n", heading);

	initMap();
	map[position.uy][position.ux] = EMPTY;

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

int getHeading() {
	int angle = 0;
	pthread_mutex_lock(&positionMutex);
	angle = heading;
	pthread_mutex_unlock(&positionMutex);
	return angle;
}

void updateMapPosition(int sonarValue) {
	int16_t x = 0, y = 0;
	pthread_mutex_lock(&positionMutex);
	x = (int16_t) getNearestInteger((position.x + (sonarValue / 10) * cos(heading * M_PI / 180)) / 5); // Work out the position of the obstacle using the current position of the robot and the value given by the sonar sensor
	y = (int16_t) getNearestInteger((position.y + (sonarValue / 10) * sin(heading * M_PI / 180)) / 5);
	if (!positionOnTheMap(x,y)) {
		printf("ERROR: The position of the obstacle is not on the map.\n");
		pthread_mutex_unlock(&positionMutex);
		return ;
	}
	map[y][x] = OBSTACLE;
	pthread_mutex_unlock(&positionMutex);
}

//Based on: http://www.robotnav.com/position-estimation/
void* update() {
	int angle, newAngle, angleDiff;
	int displacement;
	int leftWheel = 0, rightWheel = 0, leftWheelPrev = 0, rightWheelPrev = 0; // Used to store the values returned by the wheels
	int newUX, newUY;

	angle = getGyroValue();
	printf("Angle given by the gyro sensor: %d.\n",angle);

	leftWheelPrev = leftWheelPosition();
	rightWheelPrev = rightWheelPosition();

	while (updating) {
		if (!isRunning()) {
			continue;
		}
		newAngle = getGyroValue();
		//printf("Angle given by the gyro sensor: %d.\n", newAngle);

		leftWheel = leftWheelPosition();
		rightWheel = rightWheelPosition();

		displacement = (leftWheel + rightWheel - leftWheelPrev - rightWheelPrev) * (5.6 * M_PI / 360.0) / 2.0; // Work out the wheels displacement
		angleDiff = newAngle - angle; // Work out the relative rotation of the robot

		pthread_mutex_lock(&positionMutex);
		heading -=  angleDiff; // Updating the heading

		//printf("Updating the heading to %d°.\n", heading);

		position.x += displacement * cos(heading * M_PI / 180); // Trigonometric relations; to update the position
		position.y += displacement * sin(heading * M_PI / 180);
		newUX = getNearestInteger(position.x / 5);
		newUY = getNearestInteger(position.y / 5);
		//printf("Updating the position to: (x = %f, y = %f).\n", position.x, position.y);
		//printf("newUX = %d, position.ux = %d, newUY = %d, position.uy = %d\n",newUX,position.ux,newUY,position.uy);
		if (!(newUX == position.ux && newUY == position.uy)) {
			// Check if the robot changes the square (on the map)
			position.ux = newUX;
			position.uy = newUY;
			if (positionOnTheMap(position.ux,position.uy)) {
				send_position(position.ux,position.uy); // Send the position to the server
				if (map[position.uy][position.ux] == NOT_VISITED) {
					// Update the position on the map only if it's never visited
					printf("Updating the position of the robot on the map to: (x = %d, y = %d).\n", position.ux, position.uy);
					map[position.uy][position.ux] = EMPTY;
				}
			} else {
				printf("WARNING: the robot is no longer on the map: (%d,%d).\n",position.ux,position.uy);
			}
		}
		pthread_mutex_unlock(&positionMutex);

		angle = newAngle;
		leftWheelPrev = leftWheel;
		rightWheelPrev = rightWheel;
	}

}

void freePosition() {
	updating = 0;
	/*if (pthread_join(updateThread, NULL)) {
	perror("Failed to close the update thread");
	exit(EXIT_FAILURE);
}*/
}

int getNearestInteger(float f) {
	return ((int) round(f));
}

char positionOnTheMap(int x, int y) {
	return (x >= 0 && x <= MAP_WIDTH && y >= 0 && y <= MAP_HEIGHT);
}

char onTheMap() {
	char on_the_map = 0;
	pthread_mutex_lock(&positionMutex);
	on_the_map = positionOnTheMap(position.ux,position.uy);
	pthread_mutex_unlock(&positionMutex);
	return on_the_map;
}

void correctMap() {
	int numberOfEmptySquaresAround = 0, numberOfObstaclesAround = 0;

	pthread_mutex_lock(&positionMutex);
	printf("Correcting the map.\n");
	for (int y = 0; y < MAP_HEIGHT; y++) {
    for (int x = 0; x < MAP_WIDTH; x++) {
      if (map[y][x] == NOT_VISITED) {
				if (y >=1 && map[y-1][x] == EMPTY) {
					numberOfEmptySquaresAround++;
				}
				if (y >=1 && map[y-1][x] == OBSTACLE) {
					numberOfObstaclesAround++;
				}

				if (y < MAP_HEIGHT-1 && map[y+1][x] == EMPTY) {
					numberOfEmptySquaresAround++;
				}
				if (y < MAP_HEIGHT-1 && map[y+1][x] == OBSTACLE) {
					numberOfObstaclesAround++;
				}

				if (x >=1 && map[y][x-1] == EMPTY) {
					numberOfEmptySquaresAround++;
				}
				if (x >=1 && map[y][x-1] == OBSTACLE) {
					numberOfObstaclesAround++;
				}

				if (x < MAP_WIDTH && map[y][x+1] == EMPTY) {
					numberOfEmptySquaresAround++;
				}
				if (x < MAP_WIDTH && map[y][x+1] == OBSTACLE) {
					numberOfObstaclesAround++;
				}

				if (numberOfObstaclesAround >= numberOfEmptySquaresAround && numberOfObstaclesAround) {
					map[y][x] = OBSTACLE;
					printf("Correcting map(%d,%d) to OBSTACLE.\n",x,y)
				}

				if (numberOfObstaclesAround < numberOfEmptySquaresAround) {
					map[y][x] = EMPTY;
					printf("Correcting map(%d,%d) to EMPTY.\n",x,y)
				}
      }
			numberOfEmptySquaresAround = 0;
			numberOfObstaclesAround = 0;
    }
  }
	pthread_mutex_unlock(&positionMutex);
}
