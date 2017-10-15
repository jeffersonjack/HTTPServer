#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <strings.h>

#include <arpa/inet.h>

#include "http.h"
#include "util.h"

//void handleclient(int clientfd, struct sockaddr_in6 *addr)
void handleclient(int clientfd)
{
  int numbytes;
  int buffersize = 1024;
  char buffer[buffersize];
  struct request *req = malloc(sizeof(struct request));

  //printf("Got a connection from %s.\n", iptostr(addr));
  printf("Got a connection.\n");

  /* wait for a message from the client */
  if ((numbytes = read(clientfd, buffer, buffersize-1)) > 0) {
    /* echo message */
    printf("Got message:\n%s\n", buffer);
    //    /* fill buffer with null bytes */
    //    bzero(buffer, buffersize);

    if (parserequest(req, buffer, numbytes)) {
      printf("\nHTTP REQUEST\n------------\n");
      printf("Method: ");
      if (req->method == GET)
        printf("GET\n");
      else
        printf("unknown\n");
      printf("URI: %s\n", req->uri);
      printf("Version: %s\n", req->version);
      printf("\n");
    }
    else
      printf("Not a valid HTTP request.\n");

    /* send HTTP response back */
    //    numbytes = write(clientfd, "HTTP/1.1 501 Not Implemented\r\n\r\n", 32);
    //numbytes = write(clientfd, "HTTP/1.1 404 Not Found\r\n\r\nDate: Sat, 02 Feb 2013 13:42:05 CET\r\n\r\n<p>hi</p>\r\n", 76);
    numbytes = write(clientfd, "HTTP/1.1 200 OK\r\n\r\n<html>\n<body>\n<h1>Success!</h1>\n</body>\n</html>", 66);
    printf("Sent %d bytes back.\n", numbytes);
    close(clientfd);
  }
  //printf("Client %s  disconnected.\n", iptostr(addr));
  printf("Client disconnected.\n");
  close(clientfd);
}
