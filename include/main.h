#ifndef __MAIN__
#define __MAIN__

#define Sleep( msec ) usleep((msec) * 1000) // Sleep for ‘msec’ milliseconds.

#define PRINT_FILE "logs/all.txt"

extern FILE* printFile;

int getNearestInteger(float f); // Returns the nearest integer to f

#endif
