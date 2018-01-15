#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include <string.h>
#include "ev3.h"
#include "ev3_sensor.h"
#include "ev3_port.h"
#include "position.h"
#include "map.h"
#include "engines.h"
#include "sensors.h"
#include "client.h"
#include "main.h"

Position position;

char updating = 1;
char outTheMap = 0;

pthread_t updateThread;
pthread_mutex_t positionMutex;

void initPosition(float x, float y) {
	position = (Position) {x,y,getNearestInteger(x / 5),getNearestInteger(y / 5), 90};

	fprintf(printFile,"Initializing the position: x = %f, y = %f, x-MAP = %d, y-MAP = %d.\n", x, y, position.ux, position.uy);
	fprintf(printFile,"Initial heading: %d.\n", 90);

	initMap();
	map[position.uy][position.ux] = EMPTY;

	pthread_mutex_init(&positionMutex, NULL);
	if(pthread_create(&updateThread, NULL, update, NULL) == -1) {
		fprintf(printFile,"ERROR: Failed to create the update thread");
		exit(EXIT_FAILURE);
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
	angle = position.heading;
	pthread_mutex_unlock(&positionMutex);
	return angle;
}

//Based on: http://www.robotnav.com/position-estimation/
void* update() {
	int angle, newAngle, angleDiff;
	int displacement;
	int leftWheel = 0, rightWheel = 0, leftWheelPrev = 0, rightWheelPrev = 0; // Used to store the values returned by the wheels
	int newUX, newUY;

	angle = getGyroValue();
	fprintf(printFile,"Angle given by the gyro sensor: %d.\n",angle);

	leftWheelPrev = leftWheelPosition();
	rightWheelPrev = rightWheelPosition();

	while (updating) {
		if (!isRunning()) {
			continue;
		}
		newAngle = getGyroValue();
		fprintf(printFile,"Angle given by the gyro sensor: %d.\n", newAngle);

		leftWheel = leftWheelPosition();
		rightWheel = rightWheelPosition();

		displacement = (leftWheel + rightWheel - leftWheelPrev - rightWheelPrev) * (5.6 * M_PI / 360.0) / 2.0; // Work out the wheels displacement
		angleDiff = newAngle - angle; // Work out the relative rotation of the robot

		pthread_mutex_lock(&positionMutex);
		position.heading -= angleDiff; // Updating the heading

		fprintf(printFile,"Updating the heading to %d°.\n", heading);

		// If the new position has a negative coordinate
		if ((position.x + displacement * cos(position.heading * M_PI / 180)) < 0 || (position.y + displacement * sin(position.heading * M_PI / 180)) < 0) {
			fprintf(printFile,"WARNING: the robot is no longer on the map.\n");
			outTheMap = 1;
			pthread_mutex_unlock(&positionMutex);
			continue;
		}

		position.x += displacement * cos(position.heading * M_PI / 180); // Trigonometric relations; to update the position
		position.y += displacement * sin(position.heading * M_PI / 180);
		newUX = getNearestInteger(position.x / 5);
		newUY = getNearestInteger(position.y / 5);

		fprintf(printFile,"Updating the position to: (x = %f, y = %f).\n", position.x, position.y);

		// Check if the robot changes the square (on the map)
		if (!(newUX == position.ux && newUY == position.uy)) {
			position.ux = newUX;
			position.uy = newUY;
			if (positionOnTheMap(position.ux,position.uy)) {
				send_position(position.ux,position.uy); // Send the position to the server
				if (map[position.uy][position.ux] == NOT_VISITED) {
					// Update the position on the map only if it's never visited
					fprintf(printFile,"Updating the position of the robot on the map to: (x = %d, y = %d).\n", position.ux, position.uy);
					map[position.uy][position.ux] = EMPTY;
				}
			} else {
				fprintf(printFile,"WARNING: the robot is no longer on the map: (%d,%d).\n",position.ux,position.uy);
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
	if (pthread_join(updateThread, NULL)) {
		fprintf(printFile,"ERROR: Failed to close the update thread.\n");
		exit(EXIT_FAILURE);
	}
}

char positionOnTheMap(int x, int y) {
	return (x >= 0 && x <= MAP_WIDTH && y >= 0 && y <= MAP_HEIGHT);
}

char onTheMap() {
	char on_the_map = 0;
	pthread_mutex_lock(&positionMutex);
	on_the_map = positionOnTheMap(position.ux,position.uy) || outTheMap;
	pthread_mutex_unlock(&positionMutex);
	return on_the_map;
}
