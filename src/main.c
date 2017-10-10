#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <strings.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "server.h"
#include "util.h"

int main(int argc, char **argv)
{
  int portno, sock;
  char *endptr;

  /* all this stuff will eventually go in a separate file */
  int client;
  int buffersize = 1024;
  int numbytes;
  char buffer[buffersize];
  struct sockaddr_in6 *addr = malloc(sizeof(struct sockaddr_in6));
  socklen_t addrlen = sizeof(*addr);
  //  char clientaddr[INET6_ADDRSTRLEN];

  /* make sure the server was started correctly */
  if (argc != 2) {
	fprintf(stderr, "Usage: %s port\n", argv[0]);
	return 1;
  }
  /* check we were given a valid port number */
  portno = strtol(argv[1], &endptr, 10);
  if (*endptr != '\0') {
	fprintf(stderr, "Error: %s is not a valid port number.\n", argv[1]);
	return 1;
  }
  if ((portno < 1024) || (portno > 65535)) {
	fprintf(stderr, "Error: port number must be at least 1024,"
			"and no larger than 65535.\n");
	return 1;
  }

  /* get a socket to listen on */
  sock = getlisteningsocket(portno);
  
  printf("Accepting connections on port %d.\n", portno);
  while ((client = accept(sock, (struct sockaddr*) addr, &addrlen)) >= 0) {
    //	clientaddr = iptostr(addr);
    
	printf("Got a connection from %s.\n", iptostr(addr));

	while ((numbytes = read(client, buffer, buffersize-1)) > 0) {
	  printf("Got message from %s: %s", iptostr(addr), buffer);
	  /* fill buffer with null bytes */
	  bzero(buffer, buffersize);
	}
	printf("Client %s  disconnected.\n", iptostr(addr));
	close(client);
  }
  
  return 0;
}
