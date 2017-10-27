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

  int fsize = 0;
  char *filecontents = NULL;
  FILE *f;
  char *filepath = NULL;
  struct response *resp = malloc(sizeof(struct response));
  memset(resp, 0, sizeof(struct response));
  memset(resp->version, '\0', VERSIONLENGTH);

  char *responsestr = NULL;

  /* wait for a message from the client */
  if ((numbytes = read(clientfd, buffer, MAXREQSIZE-1)) > 0) {
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


    /* start making response header */
    strncpy(resp->version, "HTTP/1.1", VERSIONLENGTH-1);

    /* if we have a valid request, follow the URI */
    if (req) {
      makefilepath(req->uri, &filepath);

      /* check if the file is a directory */
      switch (directory(filepath)) {
        case DIR_OK:
          /* generate directory listing */
          fsize = 73 + strlen(req->uri);
          filecontents = malloc(fsize);
          memset(filecontents, '\0', fsize);
          strncpy(filecontents, "<html><head><title>Index</title></head><body><h1>Index of ", 58);
          strncat(filecontents, req->uri, strlen(req->uri));
          strncat(filecontents, "</h1><hr><ul>\n", 14);
          if ((fsize = getdirlist(filepath, &filecontents, fsize)) == 0)
            fprintf(stderr, "getdirlist: failed to get directory listing\n");
          /* success status */
          resp->status = malloc(7);
          memset(resp->status, '\0', 7);
          strncpy(resp->status, "200 OK", 6);
          break;
        case DIR_NOSLASH:
          /* redirect to correct directory page */
          resp->status = malloc(14);
          memset(resp->status, '\0', 14);
          strncpy(resp->status, "303 See Other", 13);
          resp_addheader(resp, "Location", strcat(req->uri, "/"));
          break;
        case 0:
          /* not a directory - a file */
          fsize = filesize(filepath);
          if (fsize >= 0) {
            /* get the file */
            filecontents = malloc(fsize);
            f = fopen(filepath, "rb");
            numbytes = fread(filecontents, sizeof(char), fsize, f);
            fclose(f);
            /* success status */
            resp->status = malloc(7 * sizeof(char));
            memset(resp->status, '\0', 7);
            strncpy(resp->status, "200 OK", 6);
            memset(buffer, '\0', MAXREQSIZE);
            sprintf(buffer, "%d", numbytes);
            resp_addheader(resp, "Content-Length", buffer);
            if (contenttype(filepath, buffer, MAXREQSIZE) == 0)
              resp_addheader(resp, "Content-Type", buffer);
          }
          break;
        default:
          /* couldn't find the file */
          resp->status = malloc(14);
          memset(resp->status, '\0', 14);
          strncpy(resp->status, "404 Not Found", 13);
          filecontents = malloc(80);
          memset(filecontents, '\0', 80);
          strncpy(filecontents, "<html><head><title>404</title></head>"
                  "<body><h1>404 Not Found</h1></body></html>", 79);
          fsize = strlen(filecontents);
          break;
      }
    }

    printf("HTTP Response\n-------------------\n");
    printf("Version: %s\n", resp->version);
    printf("Status: %s\n", resp->status);

    struct httpheader *head = resp->header;
    while (head) {
      printf("%s: %s\n", head->field, head->value);
      head = head->next;
    }

    /* generate response header string... */
    if ((numbytes = resptostr(resp, &responsestr)) < 0)
      fprintf(stderr, "Error: response couldn't be converted to a string.\n");
    else {
      /* ...and send it */
      numbytes = write(clientfd, responsestr, numbytes);
    }

    /* send the body */
    numbytes = write(clientfd, filecontents, fsize);
  }

  /* Clean up */
  if (filepath)
    free(filepath);
  free(resp->status);
  resp_freeheaders(resp);
  free(resp);
  resp = NULL;
  free(req);
  req = NULL;
  if (responsestr)
    free(responsestr);
  if (filecontents)
    free(filecontents);

  /* done: disconnect the client */
  close(clientfd);
}
