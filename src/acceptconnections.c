#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <pthread.h>

#include "server.h"

void *spawnclienthandler(void *data);

void acceptconnections(int sockfd)
{
  /* loop until we get a SIGINT (or similar) */
  while (1) {
    /* somewhere to store client data */
    struct clientinfo *client = malloc(sizeof(struct clientinfo));
    client->addrlen = sizeof(client->addr);

    if (!client) {
      perror("malloc");
      exit(1);
    }

    /* wait for, and accept, a connection */
    if ((client->clientfd = accept(sockfd, (struct sockaddr*) &(client->addr),
                                   &(client->addrlen))) >= 0) {
      /* thread for this client */
      pthread_t clientthread;

      /* do the work in a separate thread, so we can accept new clients here */
      if (pthread_create(&clientthread, NULL, spawnclienthandler, (void *) client) != 0)
        perror("pthread_create");
      if (pthread_detach(clientthread) != 0)
        fprintf(stderr, "Error: pthread_detach failed\n");
    }
    else {
      perror("accept");
      /* carry on anyway... */
    }
  }
}

void *spawnclienthandler(void *data)
{
  /* cast back to correct data type */
  struct clientinfo *client = (struct clientinfo*) data;

  /* spawn a thread */
  handleclient(client->clientfd);

  /* clean up and exit */
  shutdown(client->clientfd, SHUT_RDWR);
  free(data);
  pthread_exit(NULL);
}
