#include <unistd.h>

#ifndef __CLIENT__
#define __CLIENT__

#define SERV_ADDR "40:F0:2F:DD:B5:97"     // The address of the server
#define TEAM_ID 3                       // InSight's team ID

extern int s;
extern uint16_t msgId;

extern char receiving; // If receiving = 1, the robot can receive messages from the server as long as the latter sends them
extern char sending; // If sending = 1, the robot can send messages to the server

void initClient(); // Connecting to the server and start reading messages
void closeClient(); // This function is called when the server sends a STOP message

//Receiving
#define MSG_ACK 0
#define MSG_START 1
#define MSG_STOP 2
#define MSG_KICK 3
#define MSG_CUSTOM 4

void receive(); // Receive a message

//Sending
#define MSG_POSITION 4
#define MSG_MAPDATA 5
#define MSG_MAPDONE 6
#define MSG_OBSTACLE 7

void send_ack(uint16_t msgID, char dst); // Send the ACK message of msgID to dst
void send_position(int16_t x, int16_t y); // Sending the position (x,y)
void send_obstacle(int16_t x, int16_t y); // Informing the server that an obstacle was released
void send_map(); // Sending the map to the server
void send_done(); // Informing the server that the map was fully sent

#endif
