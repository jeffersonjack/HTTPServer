#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <arpa/inet.h>

#include "file.h"
#include "http.h"
#include "util.h"

void handleclient(int clientfd)
{
  int numbytes;
  char buffer[MAXREQSIZE];
  memset(buffer, '\0', MAXREQSIZE);
  struct request *req = malloc(sizeof(struct request));
  memset(req->version, '\0', 9);

  int fsize;
  char *filecontents = NULL;
  FILE *f;
  char *filepath = NULL;
  struct response *resp = malloc(sizeof(struct response));
  char *responsestr = NULL;

  /* wait for a message from the client */
  if ((numbytes = read(clientfd, buffer, MAXREQSIZE-1)) > 0) {
    /* echo message */
    printf("Got message:\n%s\n", buffer);

    /* try to parse an HTTP request */
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
      printf("Looking for file \"%s\"...\n", filepath);
      fsize = filesize(filepath);
      if (fsize >= 0) {
        filecontents = malloc(fsize + 1);
        f = fopen(filepath, "rb");
        printf("Size of file is %d.\n", fsize);
        while (!feof(f)) {
          numbytes = fread(filecontents, sizeof(char), fsize, f);
        }
        /* null-terminate the string */
        filecontents[fsize] = '\0';
        fclose(f);
        resp->status = malloc(7 * sizeof(char));
        memset(resp->status, '\0', 7);
        strncpy(resp->status, "200 OK", 6);
        resp->body = malloc(fsize * sizeof(int) + 1);
        if (resp->body) {
          memset(resp->body, '\0', fsize * sizeof(int) + 1);
          memcpy(resp->body, filecontents, fsize);
        }
      }
      else {
        //printf("File not found.\n");
        resp->status = malloc(14 * sizeof(char));
        strncpy(resp->status, "404 Not Found", 13);
      }
    }

    printf("HTTP Response\n-------------------\n");
    printf("Version: %s\n", resp->version);
    printf("Status: %s\n", resp->status);
    if (resp->body)
      printf("Body: \n%s\n", resp->body);

    numbytes = resptostr(resp, &responsestr);
    if (numbytes < 0)
      fprintf(stderr, "Error: response couldn't be converted to a string.\n");
    else {
      numbytes = write(clientfd, responsestr, numbytes);
      printf("Sent %d bytes back.\n", numbytes);
    }

    close(clientfd);
  }

  /* Clean up */
  if (filepath)
    free(filepath);
  free(resp->status);
  if (resp->body) {
    free(resp->body);
    resp->body = NULL;
  }
  free(resp);
  resp = NULL;
  free(req);
  req = NULL;

  if (responsestr)
    free(responsestr);

  if (filecontents)
    free(filecontents);

  printf("Client disconnected.\n");
  close(clientfd);
}
