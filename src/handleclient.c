#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <arpa/inet.h>

#include "file.h"
#include "http.h"
#include "util.h"

//void handleclient(int clientfd, struct sockaddr_in6 *addr)
void handleclient(int clientfd)
{
  int numbytes;
  int buffersize = 1024;
  char buffer[buffersize];
  struct request *req = malloc(sizeof(struct request));

  int fsize;
  char *filecontents;
  FILE *f;
  char *filepath = NULL;
  struct response *resp = malloc(sizeof(struct response));
  char *responsestr;

  //printf("Got a connection from %s.\n", iptostr(addr));
  printf("Got a connection.\n");

  /* wait for a message from the client */
  if ((numbytes = read(clientfd, buffer, buffersize-1)) > 0) {
    /* echo message */
    printf("Got message:\n%s\n", buffer);

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



    strncpy(resp->version, "HTTP/1.1\0", 9);

    /* file stuff */
    if (req) {
      makefilepath(req->uri, &filepath);
      //printf("Looking for file \"%s\"...\n", filepath);
      fsize = filesize(filepath);
      if (fsize >= 0) {
        filecontents = malloc((fsize * sizeof(char)) + 1);
        f = fopen(filepath, "r");
        numbytes = fread(filecontents, sizeof(char), fsize, f);
        /* null-terminate the string */
        filecontents[fsize] = '\0';
        //printf("Found:\n---------------------\n%s\n---------------------\n", filecontents);
        fclose(f);
        resp->status = malloc(7 * sizeof(char));
        memset(resp->status, '\0', 7);
        strncpy(resp->status, "200 OK", 6);
        resp->body = malloc(fsize * sizeof(int) + 1);
        strncpy(resp->body, filecontents, fsize);
      }
      else {
        printf("File not found.\n");
        resp->status = malloc(14 * sizeof(char));
        strncpy(resp->status, "404 Not Found", 13);
      }
    }

    printf("HTTP Response\n-------------------\n");
    printf("Version: %s\n", resp->version);
    printf("Status: %s\n", resp->status);
    if (resp->body)
      printf("Body: \n%s\n", resp->body);


    /* send HTTP response back */
    //    numbytes = write(clientfd, "HTTP/1.1 501 Not Implemented\r\n\r\n", 32);
    //numbytes = write(clientfd, "HTTP/1.1 404 Not Found\r\n\r\nDate: Sat, 02 Feb 2013 13:42:05 CET\r\n\r\n<p>hi</p>\r\n", 76);
    //numbytes = write(clientfd, "HTTP/1.1 200 OK\r\n\r\n<html>\n<body>\n<h1>Success!</h1>\n</body>\n</html>", 66);

    numbytes = resptostr(resp, &responsestr);
    // TODO: check numbytes

    numbytes = write(clientfd, responsestr, numbytes);

    printf("Sent %d bytes back.\n", numbytes);
    close(clientfd);
  }

  /* Clean up */
  free(resp->status);
  if (resp->body) {
    free(resp->body);
    resp->body = NULL;
  }
  free(resp);
  resp = NULL;

  //printf("Client %s  disconnected.\n", iptostr(addr));
  printf("Client disconnected.\n");
  close(clientfd);
}
