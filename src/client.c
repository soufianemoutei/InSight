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
  receiving = 0; // Stop receiving
  exploring = 0; // Stop exploring
  if (updating != 0) { // Stop updating the position
    freePosition();
  }
  stopRunning(); // Stop running
  close(s); // Close the socket
}

void receive() {
  char string[58];

  int bytes_read = read(s, string, 58);

  if (bytes_read <= 0) {
    fprintf (stderr, "Server unexpectedly closed connection...\n");
    closeClient();
    exit (EXIT_FAILURE);
  }

  printf("Received %d bytes\n", bytes_read);

  if (string[2] != 0xFF) {
    send_ack(*((uint16_t *) string), string[2]);
  }

  switch (string[4]) {
    case MSG_ACK:
    printf("ACK Message was received; State:%s\n",(string[7] == 0 ? "OK" : "Error"));
    break;

    case MSG_START:
    printf("START Message was received.\n");
    sending = 1; // It means we allow the robot to send messages to the server
    explore(); // Explore the map
    break;

    case MSG_STOP:
    printf("STOP Message was received.\n");
    closeClient();
    break;

    case MSG_KICK:
    printf("The robot %d was kicked.\n",string[5]);
    break;

    case MSG_CUSTOM:
    printf("CUSTOM Message was received from %d: %s\n",string[2],(string+5));
    break;

    default:
    printf("ERROR: THE TYPE OF THE RECEIVED MESSAGE IS UNKNOWN.\n");
    break;

  }
}

void send_ack(uint16_t msgID, char dst) {
  char string[8];
  if (!sending) {
    return ;
  }
  printf("Sending ACK Message to %d.\n",dst);
  *((uint16_t *) string) = msgId++; // ID
  string[2] = TEAM_ID; // Source
  string[3] = dst; // Destination
  string[4] = MSG_ACK;
  string[5] = (uint8_t) msgID; // The ID of the message that is acknowledged
  string[6] = (uint8_t) (msgID >> 8);
  string[7] = 0; // Status
  write(s, string, 8);
}

void send_obstacle(int16_t x, int16_t y) {
  char string[10];
  if (!sending) {
    return ;
  }
  printf("Sending OBSTACLE Message (%d,%d) to the server.\n",x,y);
  *((uint16_t *) string) = msgId++; // ID
  string[2] = TEAM_ID; // Source
  string[3] = 0xFF; // Destination = Server
  string[4] = MSG_OBSTACLE;
  string[5] = 0; // Indicate that the robot released the object
  string[6] = (uint8_t) x; // The x-position of the obstacle
  string[7] = (uint8_t) (x >> 8);
  string[8] = (uint8_t) y; // The y-position of the obstacle
  string[9]= (uint8_t) (y >> 8);
  write(s, string, 10);
}

void send_position(int16_t x, int16_t y) {
  char string[9];
  if (!sending) {
    return ;
  }
  printf("Sending the position (%d,%d) to the server.\n",x,y);
  *((uint16_t *) string) = msgId++; // ID
  string[2] = TEAM_ID; // Source
  string[3] = 0xFF; // Destination = Server
  string[4] = MSG_POSITION;
  string[5] = (uint8_t) x; // The x-position of the robot
  string[6] = (uint8_t) (x >> 8);
  string[7] = (uint8_t) y; // The y-position of the robot
  string[8]= (uint8_t) (y >> 8);
  write(s, string, 9);
}

void send_map() {
  char string[12];
  if (!sending) {
    return ;
  }
  correctMap(); // Correct the map before sending it to the server
  printf ("Sending the map to the server...\n");
  for (int y = 0; y < MAP_HEIGHT; y++) {
    for (int x = 0; x < MAP_WIDTH; x++) {
      if (map[y][x] != NOT_VISITED) {
        printf("The state of the position (%d,%d) is: %s.\n",x,y,(map[y][x] == EMPTY ? "EMPTY" : "OBSTACLE"));
      }
      *((uint16_t *) string) = msgId++; // ID
      string[2] = TEAM_ID; // Source
      string[3] = 0xFF; // Destination = Server
      string[4] = MSG_MAPDATA;
      string[5] = (uint8_t) x; // The x-position of the map
      string[6] = (uint8_t) (x >> 8);
      string[7] = (uint8_t) y; // The y-position of the map
      string[8]= (uint8_t) (y >> 8);
      if (map[y][x] == OBSTACLE) { // Indicate the obstacle position with a red square
        string[9] = 200;
        string[10] = 0;
        string[11] = 0;
      } else if (map[y][x] == EMPTY) { // Indicate the empty position with a white square
        string[9] = 255;
        string[10] = 255;
        string[11] = 255;
      } else { // Indicate the non-visited position with a gray square
        string[9] = 128;
        string[10] = 128;
        string[11] = 128;
      }
      write(s, string, 12);
      Sleep(10);
    }
  }
  send_done(); // The map is finished
}

void send_done() {
  char string[5];
  printf("The map is finished.\n");
  *((uint16_t *) string) = msgId++; // ID
  string[2] = TEAM_ID; // Source
  string[3] = 0xFF; // Destination = Server
  string[4] = MSG_MAPDONE;
  write(s, string, 5);
}
