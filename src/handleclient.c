#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <time.h>

#include "file.h"
#include "http.h"

void handleclient(int clientfd)
{
  int numbytes = 0;
  int fsize = 0;
  char buffer[MAXREQLEN];
  struct request *req;
  struct response *resp;
  char *filecontents = NULL;
  char *filepath = NULL;
  char *responsestr = NULL;
  char *timestr= NULL;
  time_t seconds;

  /* initialise request structure */
  req = malloc(sizeof(struct request));
  memset(req->version, '\0', 9);

  /* initialise response structure */
  resp = malloc(sizeof(struct response));
  memset(resp, 0, sizeof(struct response));
  memset(resp->version, '\0', VERSIONLEN);

  /* initialise buffers */
  memset(buffer, '\0', MAXREQLEN);

  /* wait for a message from the client */
  if ((numbytes = read(clientfd, buffer, MAXREQLEN-1)) > 0) {
    /* try to parse an HTTP request */
    if (!parserequest(req, buffer, numbytes))
      fprintf(stderr, "Error: received bad request\n");
    else
      printrequest(req);

    /* start making response header */
    strncpy(resp->version, "HTTP/1.1", VERSIONLEN-1);

    /* if we have a valid request, follow the URI */
    if (req) {
      makefilepath(req->uri, &filepath);

      /* check if the file is a directory */
      switch (directory(filepath)) {
        case DIR_OK:
          /* generate response and directory listing */
          fsize = makeresponse_dir(resp, req->uri, filepath, &filecontents);
          break;
        case DIR_NOSLASH:
          /* redirect to correct directory page */
          resp->status = malloc(14);
          memset(resp->status, '\0', 14);
          strncpy(resp->status, "303 See Other", 13);
          resp_addheader(resp, "Location", strcat(req->uri, "/"));
          break;
        case 0:
          /* get the file and generate an HTTP response */
          fsize = filesize(filepath);
          if (fsize >= 0) {
            makeresponse_file(resp, filepath, &filecontents);
          }
          break;
        default:
          /* couldn't find the file */
          fsize = makeresponse_404(resp, &filecontents);
          break;
      }
    }

    /* add a timestamp */
    timestr = malloc(TIMESTRLEN);
    memset(timestr, '\0', TIMESTRLEN);
    seconds = time(0);    /* time now */
    httptime(&seconds, timestr);
    resp_addheader(resp, "Date", timestr);

    printresponse(resp);

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
  if (timestr)
    free(timestr);
  if (responsestr)
    free(responsestr);
  if (filecontents)
    free(filecontents);

  /* done: disconnect the client */
  close(clientfd);
}
