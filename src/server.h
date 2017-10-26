#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>

/* address information of clients that connect */
struct clientinfo {
  int clientfd;
  struct sockaddr_in6 addr;
  socklen_t addrlen;
};

/* gets a socket on the given port and listen for connections. return a file
   descriptor of the socket */
int getlisteningsocket(int port);

/* accept connections on the given socket */
void acceptconnections(int socket);

/* read data from the client */
void handleclient(int client);

#endif /* SERVER_H */
