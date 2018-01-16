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
#include "main.h"

State map[MAP_HEIGHT][MAP_WIDTH];

void initMap() {
	fprintf(printFile,"Initializing the map.\n"); // With NOT_VISITED
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			map[i][j] = NOT_VISITED;
		}
	}
}

void updateMapPosition(int sonarValue) {
	int16_t x = 0, y = 0;
	pthread_mutex_lock(&positionMutex);
	x = (int16_t) getNearestInteger((position.x + (sonarValue / 10) * cos(position.heading * M_PI / 180)) / 5); // Work out the position of the obstacle using the current position of the robot and the value given by the sonar sensor
	y = (int16_t) getNearestInteger((position.y + (sonarValue / 10) * sin(position.heading * M_PI / 180)) / 5);
	if (!positionOnTheMap(x,y)) {
		fprintf(printFile,"ERROR: The position of the obstacle is not on the map.\n");
		pthread_mutex_unlock(&positionMutex);
		return ;
	}
	map[y][x] = OBSTACLE;
	pthread_mutex_unlock(&positionMutex);
}

void correctMap() {
	// 1st round of correcting the map: Eliminating non-visited squares by voting
	fprintf(printFile,"Correcting the map: 1st round.\n");
	correctMapByVoting();
	printMap(PRINT_FILE);

	// 2nd round of correcting the map: Eliminating empty squares surrounded by three obstacles
	fprintf(printFile,"Correcting the map: 2nd round.\n");
	correctEmptySquaresMap();
	printMap(PRINT_FILE);

	// 3rd round of correcting the map: Eliminating again non-visited squares by voting
	fprintf(printFile,"Correcting the map: 3rd round.\n");
	correctMapByVoting();
	printMap(PRINT_FILE);

	// 4th round of correcting the map: Substituting non-visited squares by empty squares
	fprintf(printFile,"Correcting the map: 4th round.\n");
	correctMapByEliminatingNonVisitedSquares();
	printMap(PRINT_FILE);

	// Saving the map in a file
	printMap(MAP_FILE);
}

void correctMapByVoting() {
	int numberOfEmptySquaresAround = 0, numberOfObstaclesAround = 0, numberOfNonVisistedAround = 0;

	pthread_mutex_lock(&positionMutex);

	for (int y = 0; y < MAP_HEIGHT; y++) {
		for (int x = 0; x < MAP_WIDTH; x++) {
			if (map[y][x] == NOT_VISITED) { // If (x,y) is not visited

				// The position on (x,y-1)
				if (y >=1 && map[y-1][x] == EMPTY) {
					numberOfEmptySquaresAround++;
				}
				if (y >=1 && map[y-1][x] == OBSTACLE) {
					numberOfObstaclesAround++;
				}
				if (y >=1 && map[y-1][x] == NOT_VISITED) {
					numberOfNonVisistedAround++;
				}

				// The position on (x,y+1)
				if (y < MAP_HEIGHT-1 && map[y+1][x] == EMPTY) {
					numberOfEmptySquaresAround++;
				}
				if (y < MAP_HEIGHT-1 && map[y+1][x] == OBSTACLE) {
					numberOfObstaclesAround++;
				}
				if (y < MAP_HEIGHT-1 && map[y+1][x] == NOT_VISITED) {
					numberOfNonVisistedAround++;
				}

				// The position on (x-1,y)
				if (x >=1 && map[y][x-1] == EMPTY) {
					numberOfEmptySquaresAround++;
				}
				if (x >=1 && map[y][x-1] == OBSTACLE) {
					numberOfObstaclesAround++;
				}
				if (x >=1 && map[y][x-1] == NOT_VISITED) {
					numberOfNonVisistedAround++;
				}

				// The position on (x+1,y)
				if (x < MAP_WIDTH-1 && map[y][x+1] == EMPTY) {
					numberOfEmptySquaresAround++;
				}
				if (x < MAP_WIDTH-1 && map[y][x+1] == OBSTACLE) {
					numberOfObstaclesAround++;
				}
				if (x < MAP_WIDTH-1 && map[y][x+1] == NOT_VISITED) {
					numberOfNonVisistedAround++;
				}

				// If the number of obstacles around (x,y) is larger than the number of empty squares and the number of non-visited squares, change (x,y) to an obstacle
				if (numberOfObstaclesAround >= numberOfNonVisistedAround && numberOfObstaclesAround > numberOfEmptySquaresAround && numberOfObstaclesAround) {
					map[y][x] = OBSTACLE;
					fprintf(printFile,"Correcting map(%d,%d) to OBSTACLE.\n",x,y);
				}

				// If the number of empty squares around (x,y) is larger than the number of obstacles and the number of non-visited squares, change (x,y) to an empty space
				if (numberOfEmptySquaresAround >= numberOfNonVisistedAround && numberOfObstaclesAround <= numberOfEmptySquaresAround) {
					map[y][x] = EMPTY;
					fprintf(printFile,"Correcting map(%d,%d) to EMPTY.\n",x,y);
				}
			}
			numberOfEmptySquaresAround = 0;
			numberOfObstaclesAround = 0;
			numberOfNonVisistedAround = 0;
		}
	}
	pthread_mutex_unlock(&positionMutex);
}

void correctEmptySquaresMap() {
	int numberOfObstaclesAround = 0;

	pthread_mutex_lock(&positionMutex);

	for (int y = 0; y < MAP_HEIGHT; y++) {
		for (int x = 0; x < MAP_WIDTH; x++) {
			if (map[y][x] == EMPTY) { // If (x,y) is empty

				// The position on (x,y-1)
				if (y >=1 && map[y-1][x] == OBSTACLE) {
					numberOfObstaclesAround++;
				}

				// The position on (x,y+1)
				if (y < MAP_HEIGHT-1 && map[y+1][x] == OBSTACLE) {
					numberOfObstaclesAround++;
				}

				// The position on (x-1,y)
				if (x >=1 && map[y][x-1] == OBSTACLE) {
					numberOfObstaclesAround++;
				}

				// The position on (x+1,y)
				if (x < MAP_WIDTH-1 && map[y][x+1] == OBSTACLE) {
					numberOfObstaclesAround++;
				}

				// If there is an empty square between three obstacles, change this square to an obstacle
				if (numberOfObstaclesAround >= 3) {
					map[y][x] = OBSTACLE;
					fprintf(printFile,"Correcting map(%d,%d) to OBSTACLE.\n",x,y);
				}
			}
			numberOfObstaclesAround = 0;
		}
	}
	pthread_mutex_unlock(&positionMutex);
}

void correctMapByEliminatingNonVisitedSquares() {
	pthread_mutex_lock(&positionMutex);
	for (int y = 0; y < MAP_HEIGHT; y++) {
		for (int x = 0; x < MAP_WIDTH; x++) {
			if (map[y][x] == NOT_VISITED) { // If (x,y) is NOT_VISITED
				map[y][x] = EMPTY;
				fprintf(printFile,"Correcting map(%d,%d) to EMPTY.\n",x,y);
			}
		}
	}
	pthread_mutex_unlock(&positionMutex);
}

void printMap(path_file) {

	if (path_file == NULL || strlen(path_file) == 0) {
		FILE* mapFile = stdout;
	} else {
		FILE* mapFile = fopen(path_file, "w+");
	}

	pthread_mutex_lock(&positionMutex);

	for (int y = 0; y < MAP_HEIGHT; y++) {
		for (int x = 0; x < MAP_WIDTH; x++) {
			if (map[y][x] == NOT_VISITED) {
				fprintf(mapFile,"?");
			} else if (map[y][x] == EMPTY) {
				fprintf(mapFile,"-");
			} else {
				fprintf(mapFile,"X");
			}
		}
		fprintf(mapFile,"\n");
	}

	pthread_mutex_unlock(&positionMutex);

	if (!(path_file == NULL || strlen(path_file) == 0)) {
		fclose(mapFile);
	}
}

void addSmallArenaBounds() {
	pthread_mutex_lock(&positionMutex);

	for (int x = 0; x < MAP_WIDTH; x++) {
		map[0][x] = OBSTACLE;
	}

	for (int x = 0; x < MAP_WIDTH; x++) {
		map[MAP_HEIGHT-1][x] = OBSTACLE;
	}

	for (int y = 0; y < MAP_HEIGHT; y++) {
		map[y][0] = OBSTACLE;
	}

	for (int y = 0; y < MAP_HEIGHT; y++) {
		map[y][MAP_WIDTH-1] = OBSTACLE;
	}

	pthread_mutex_unlock(&positionMutex);
}
