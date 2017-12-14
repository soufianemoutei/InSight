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
uint16_t msgId = 0;

pthread_t sendPositionThread;
int sending = 1;

void initClient() {
  struct sockaddr_rc addr = {0};
  
  /* allocate a socket */
  s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
  
  /* set the connection parameters (who to connect to) */
  addr.rc_family = AF_BLUETOOTH;
  addr.rc_channel = (uint8_t) 1;
  str2ba (SERV_ADDR, &addr.rc_bdaddr);
  
  /* connect to server */
  
  if (!connect(s, (struct sockaddr *)&addr, sizeof(addr))) {
    receive();
  } else {
    fprintf (stderr, "Failed to connect to server...\n");
    sleep(2);
    exit(EXIT_FAILURE);
  }
}

void closeClient() {
  sending = 0;
  if (pthread_join(sendPositionThread, NULL)) {
    perror("pthread_join");
    exit(EXIT_FAILURE);
  }
  close(s);
  
}

int receive() {
  char string[58];
  
  int bytes_read = read(s, string, 9);
  
  if (bytes_read <= 0) {
    fprintf (stderr, "Server unexpectedly closed connection...\n");
    close (s);
    exit (EXIT_FAILURE);
  }
  
  printf("[DEBUG] received %d bytes\n", bytes_read);
  printf("SRC:%c\n",string[2]);
  printf("DST:%c\n",string[3]);
  
  switch (string[4]) {
    case MSG_ACK:
    printf("ACK\n");
    printf("STATE:%s\n",(string[7] == 0 ? "OK" : "Error"));
    break;
    
    case MSG_START:
    printf("START\n");
    sendToServer();
    break;
    
    case MSG_STOP:
    printf("STOP\n");
    closeClient();
    break;
    
    case MSG_KICK:
    printf("The robot %d was kicked\n",string[5]);
    break;
    
    case MSG_CUSTOM:
    printf("MSG:%c\n",*(string+5));
    break;
    
    default:
    printf("ERROR, MESSAGE SENT WAS: %s",string);
    break;
    
  }
  return bytes_read;
}

void sendToServer() {
  
  printf ("I'm navigating...\n");
  
  explore();
  
  srand(time(NULL));
  
  if(pthread_create(&updateThread, NULL, send_position, NULL) == -1) {
    printf("pthread_create");
    exit(EXIT_FAILURE);
  }
  
  //send_map();
  //send_done();
  
  printf("I'm waiting for the stop message");
}

void* send_position() {
  char string[58];
  /* Send 30 POSITION messages, a BALL message, 1 position message, then a NEXT message */
  float x, y;
  uint16_t xInt, yInt;
  
  while (sending) {
    getPosition(&x,&y);
    xInt = (uint16_t) x;
    yInt = (uint16_t) y;
    
    *string = msgId++;
    string[2] = TEAM_ID;
    string[3] = 0xFF;
    string[4] = MSG_POSITION;
    string[5] = (uint8_t) xInt;          /* x */
    string[6] = (uint8_t) (xInt >> 8);
    string[7] = (uint8_t) yInt;		/* y */
    string[8]= (uint8_t) (yInt >> 8);
    write(s, string, 9);
    sleep(2);
  }
}

void send_map() {
  char string[58];
  int x1, x2, y1, y2;
  printf ("I'm sending my map...\n");
  
  /* MAP data is in the form MAPDATA | X  X |Y  Y | R | G | B */
  
  /* Create 1 square green obstacle and 1 round red obstacle*/
  /* Send only the outline */
  x1 = rand() % 30;
  y1= rand() % 30;
  
  for (int i=x1; i<x1+10; i++){
    *string = msgId++;
    string[2] = TEAM_ID;
    string[3] = 0xFF;
    string[4] = MSG_MAPDATA;
    string[5] = i;          /* x */
    string[6] = 0x00;
    string[7] = y1;		/* y */
    string[8]= 0x00;
    string[9]= 0;
    string[10]=254;
    string[11]= 0;
    write(s, string, 12);
    Sleep( 100 );
  }
  for (int i=x1; i<x1+10; i++){
    *string = msgId++;
    string[2] = TEAM_ID;
    string[3] = 0xFF;
    string[4] = MSG_MAPDATA;
    string[5] = i;          /* x */
    string[6] = 0x00;
    string[7] = y1+10;		/* y */
    string[8]= 0x00;
    string[9]= 0;
    string[10]=254;
    string[11]= 0;
    write(s, string, 12);
    Sleep( 100 );
  }
  for (int j=y1; j<y1+10; j++){
    *string = msgId++;
    string[2] = TEAM_ID;
    string[3] = 0xFF;
    string[4] = MSG_MAPDATA;
    string[5] = x1;          /* x */
    string[6] = 0x00;
    string[7] = j;		/* y */
    string[8]= 0x00;
    string[9]= 0;
    string[10]=254;
    string[11]= 0;
    write(s, string, 12);
    Sleep( 100 );
  }
  
  for (int j=y1; j<y1+10; j++){
    *string = msgId++;
    string[2] = TEAM_ID;
    string[3] = 0xFF;
    string[4] = MSG_MAPDATA;
    string[5] = x1+10;          /* x */
    string[6] = 0x00;
    string[7] = j;		/* y */
    string[8]= 0x00;
    string[9]= 0;
    string[10]=254;
    string[11]= 0;
    write(s, string, 12);
    Sleep( 100 );
  }
  
  x2 = 15 +rand() % 20;
  y2= 15 + rand() % 20;
  
  
  for (int i=x2-15; i<x2+15; i++){
    for (int j=y2-15; j<y2+15; j++){
      if (sqrt((i-x2)*(i-x2) + (j-y2)*(j-y2)) < 15 && sqrt((i-x2)*(i-x2) + (j-y2)*(j-y2))>14){
        *string = msgId++;
        string[2] = TEAM_ID;
        string[3] = 0xFF;
        string[4] = MSG_MAPDATA;
        string[5] = i;          /* x */
        string[6] = 0x00;
        string[7] = j;		/* y */
        string[8]= 0x00;
        string[9]= 254;
        string[10]=0;
        string[11]= 0;
        write(s, string, 12);
        Sleep( 100 );
      }
    }
  }
}

void send_done() {
  char string[58];
  printf("Done sending map");
  *string = msgId++;
  string[2] = TEAM_ID;
  string[3] = 0xFF;
  string[4] = MSG_MAPDONE;
  write(s, string, 5);
}
