#include <stdlib.h>

#include "server.h"

void acceptconnections(int sockfd)
{
  int client;
  struct sockaddr_in6 *addr = malloc(sizeof(struct sockaddr_in6));
  socklen_t addrlen = sizeof(*addr);

  /* loop forever, accepting a connection every time */
  while ((client = accept(sockfd, (struct sockaddr*) addr, &addrlen)) >= 0)
    handleclient(client, addr);
}
