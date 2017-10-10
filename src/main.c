#include <stdlib.h>
#include <stdio.h>

#include "server.h"

int main(int argc, char **argv)
{
  int portno, sock;
  char *endptr;

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

  /* get a socket to listen on and listen on it */
  sock = getlisteningsocket(portno);
  printf("Accepting connections on port %d.\n", portno);
  acceptconnections(sock);

  return 0;
}
