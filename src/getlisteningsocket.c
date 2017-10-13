#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>

int getlisteningsocket(int port)
{
  struct sockaddr_in6 *address;
  int sock;

  address = malloc(sizeof(struct sockaddr_in6));

  /* set address to zero, as per the netinet_in.h manpage */
  memset(address, '\0', sizeof(*address));
  address->sin6_family = AF_INET6;
  address->sin6_port = htons(port);
  address->sin6_addr = in6addr_any;

  /* ask for a socket and check we received one */
  sock = socket(PF_INET6, SOCK_STREAM, 0);
  if (sock < 0) {
    perror("socket");
    exit(-1);
  }

  /* bind the address to the socket and check for success */
  if (bind(sock, (struct sockaddr*) address, sizeof(*address))) {
    perror("bind");
    exit(-1);
  }

  /* listen for socket connections. backlog of 5, just because */
  if (listen(sock, 5)) {
    perror("listen");
    exit(-1);
  }

  /* return the socket file descriptor to caller */
  return sock;
}
