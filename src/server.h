#ifndef SERVER_H
#define SERVER_H

/* gets a socket on the given port and listen for connections.
 * return a file descriptor of the socket */
int getlisteningsocket(int port);

#endif /* SERVER_H */
