#include <unistd.h>
#include <pthread.h>

#ifndef __CLIENT__
#define __CLIENT__

#define SERV_ADDR "A0:E6:F8:FC:BB:10"     /* Whatever the address of the server is */
#define TEAM_ID 4                       /* Your team ID */

extern int s;
extern uint16_t msgId;

extern pthread_t sendPositionThread;
extern int sending;

void initClient();
void closeClient();

//Receiving
#define MSG_ACK 0
#define MSG_START 1
#define MSG_STOP 2
#define MSG_KICK 3
#define MSG_CUSTOM 4

int receive();

//Sending
#define MSG_POSITION 4
#define MSG_MAPDATA 5
#define MSG_MAPDONE 6

void sendToServer();
void* send_position();
void send_map();
void send_done();

#endif
