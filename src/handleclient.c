#include <stdio.h>
#include <unistd.h>
#include <strings.h>

#include <arpa/inet.h>

#include "util.h"

//void handleclient(int clientfd, struct sockaddr_in6 *addr)
void handleclient(int clientfd)
{
  int numbytes;
  int buffersize = 1024;
  char buffer[buffersize];

  //printf("Got a connection from %s.\n", iptostr(addr));
  printf("Got a connection.\n");

  /* read messages until the client disconnects */
  while ((numbytes = read(clientfd, buffer, buffersize-1)) > 0) {
    /* echo message */
    //printf("Got message from %s: %s", iptostr(addr), buffer);
    printf("Got message: %s\n", buffer);
    /* fill buffer with null bytes */
    bzero(buffer, buffersize);
  }
  //printf("Client %s  disconnected.\n", iptostr(addr));
  printf("Client disconnected.\n");
  close(clientfd);
}
