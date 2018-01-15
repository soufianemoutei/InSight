#ifndef __MAP__
#define __MAP__

#define MAP_HEIGHT 80 // The map height (40 for small arena)
#define MAP_WIDTH 80 // The map width (24 for small arena)
#define MAP_FILE "logs/map.txt"

typedef enum State {
  NOT_VISITED = 0, // Unknown state
  EMPTY = 1, // Empty, the robot can visit it
  OBSTACLE = 2 // Obstacle
} State;

extern State map[MAP_HEIGHT][MAP_WIDTH]; // The map of the arena. It's a matrix whose elements are States: NOT_VISITED, EMPTY (the robot can visit it or there is just a movable obstacle), OBSTACLE (non-movable obstacles)

void initMap(); // Init the map with NOT_VISITED
void updateMapPosition(int sonarValue); // It's called when a non-movable obstacle was found, it updates the position of the obstacle on the map with the value OBSTACLE
void correctMap(); // Corrects the non-visited squares on the map
void correctMapByVoting(); // Corrects the non-visited squares on the map by voting (we look at the squares around).
void correctEmptySquaresMap(); // Corrects the map: Eliminating empty squares surrounded by three obstacles
void correctMapByEliminatingNonVisitedSquares(); //  Corrects the non-visited squares on the map: Substituting non-visited squares by empty squares
void printMap(path_file); // Saves the map in a file or print in it if ‘path_file’ is NULL;
void addSmallArenaBounds(); // Add Small Arena bounds

#endif
