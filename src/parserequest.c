#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "http.h"

int parserequest(struct request *req, char *data, int size)
{
  assert(req != NULL);

  char *token;
  char str[size];

  strncpy(str, data, size);
  token = strtok(str, " ");

  /* first bit is the method */
  if (strcmp(token, "GET") == 0)
    req->method = GET;
  else
    return 0;    /* fail */

  /* next comes the URI */
  token = strtok(NULL, " \0");
  strncpy(req->uri, token, MAXURISIZE-1);

  /* then the HTTP version */
  token = strtok(NULL, " \0");
  strncpy(req->version, token, 8);

  /* success */
  return 1;
}
