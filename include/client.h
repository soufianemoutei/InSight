#include <unistd.h>

#ifndef __CLIENT__
#define __CLIENT__

#define SERV_ADDR "40:F0:2F:DD:B5:97"     /* Whatever the address of the server is */
#define TEAM_ID 3                       /* Your team ID */

extern int s;
extern uint16_t msgId;

extern char receiving;
extern char sending;

void initClient();
void closeClient();

//Receiving
#define MSG_ACK 0
#define MSG_START 1
#define MSG_STOP 2
#define MSG_KICK 3
#define MSG_CUSTOM 4

void receive();

//Sending
#define MSG_POSITION 4
#define MSG_MAPDATA 5
#define MSG_MAPDONE 6
#define MSG_OBSTACLE 7

void send_ack(uint16_t msgID, char dst);
void send_position(int16_t x, int16_t y);
void send_obstacle(int16_t x, int16_t y);
void send_map();
void send_done();

#endif
