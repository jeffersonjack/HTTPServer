#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>

/* gets a socket on the given port and listen for connections.
 * return a file descriptor of the socket */
int getlisteningsocket(int port);

/* accept connections on the given socket */
void acceptconnections(int socket);

/* read data from the client */
void handleclient(int client, struct sockaddr_in6 *addr);

#endif /* SERVER_H */
