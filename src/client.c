#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <time.h>
#include <sys/socket.h>
#include <math.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <pthread.h>
#include "client.h"
#include "engines.h"
#include "position.h"

int s;
uint16_t msgId = 0x0000;

char sending = 0, receiving = 1;

void initClient() {
  struct sockaddr_rc addr = {0};
  int status;

  /* allocate a socket */
  s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

  /* set the connection parameters (who to connect to) */
  addr.rc_family = AF_BLUETOOTH;
  addr.rc_channel = (uint8_t) 1;
  str2ba (SERV_ADDR, &addr.rc_bdaddr);

  /* connect to server */
  status = connect(s, (struct sockaddr *)&addr, sizeof(addr));

  if (!status) {
    while (receiving) {
      receive();
    }
  } else {
    fprintf (stderr, "Failed to connect to server...\n");
    sleep(2);
    exit(EXIT_FAILURE);
  }
}

void closeClient() {
  receiving = 0;
  exploring = 0;
  if (updating != 0) {
    freePosition();
  }
  stopRunning();
  close(s);
}

void receive() {
  char string[58];

  int bytes_read = read(s, string, 9);

  if (bytes_read <= 0) {
    fprintf (stderr, "Server unexpectedly closed connection...\n");
    close (s);
    exit (EXIT_FAILURE);
  }

  printf("Received %d bytes\n", bytes_read);
  send_ack(*((uint16_t *) string), string[2]);

  switch (string[4]) {
    case MSG_ACK:
    printf("ACK Message\n");
    printf("STATE:%s\n",(string[7] == 0 ? "OK" : "Error"));
    break;

    case MSG_START:
    printf("START Message\n");
    sending = 1;
    explore();
    break;

    case MSG_STOP:
    printf("STOP Message\n");
    closeClient();
    break;

    case MSG_KICK:
    printf("The robot %d was kicked\n",string[5]);
    break;

    case MSG_CUSTOM:
    printf("MSG:%s\n",string);
    break;

    default:
    printf("ERROR, MESSAGE SENT WAS: %s",string);
    break;

  }
}

void send_ack(uint16_t msgID, char dst) {
  char string[8];
  if (!sending) {
    return ;
  }
  printf("SENDING ACK MESSAGE TO %d\n",dst);
  *((uint16_t *) string) = msgId++;
  string[2] = TEAM_ID;
  string[3] = dst;
  string[4] = MSG_ACK;
  string[5] = (uint8_t) msgID;
  string[6] = (uint8_t) (msgID >> 8);
  string[7] = 0;
  write(s, string, 8);
}

void send_obstacle(int16_t x, int16_t y) {
  char string[10];
  if (!sending) {
    return ;
  }
  printf("SENDING OBSTACLE MESSAGE (%d,%d) TO THE SERVER\n",x,y);
  *((uint16_t *) string) = msgId++;
  string[2] = TEAM_ID;
  string[3] = 0xFF;
  string[4] = MSG_OBSTACLE;
  string[5] = 0;
  string[6] = (uint8_t) x;
  string[7] = (uint8_t) (x >> 8);
  string[8] = (uint8_t) y;
  string[9]= (uint8_t) (y >> 8);
  write(s, string, 10);
}

void send_position(int16_t x, int16_t y) {
  char string[9];
  if (!sending) {
    return ;
  }
  printf("SENDING POSITION (%d,%d) TO THE SERVER\n",x,y);
  *((uint16_t *) string) = msgId++;
  string[2] = TEAM_ID;
  string[3] = 0xFF;
  string[4] = MSG_POSITION;
  string[5] = (uint8_t) x;
  string[6] = (uint8_t) (x >> 8);
  string[7] = (uint8_t) y;
  string[8]= (uint8_t) (y >> 8);
  write(s, string, 9);
}

void send_map() {
  char string[12];
  if (!sending) {
    return ;
  }
  printf ("SENDING THE MAP...\n");
  for (int y; y < MAP_HEIGHT; y++) {
    for (int x; x < MAP_WIDTH; x++) {
      *((uint16_t *) string) = msgId++;
      string[2] = TEAM_ID;
      string[3] = 0xFF;
      string[4] = MSG_MAPDATA;
      string[5] = (uint8_t) x;
      string[6] = (uint8_t) (x >> 8);
      string[7] = (uint8_t) y;
      string[8]= (uint8_t) (y >> 8);
      string[9]= 0;
      string[10] = ((map[x][y] == OBSTACLE) ? 0 : 254);
      string[11] = 0;
      write(s, string, 12);
    }
  }
  send_done();
}

void send_done() {
  char string[5];
  if (!sending) {
    return ;
  }
  printf("DONE SENDING MAP");
  *((uint16_t *) string) = msgId++;
  string[2] = TEAM_ID;
  string[3] = 0xFF;
  string[4] = MSG_MAPDONE;
  write(s, string, 5);
}
