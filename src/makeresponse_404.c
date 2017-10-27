#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "http.h"

int makeresponse_404(struct response *resp, char **body)
{
  int bodysize;
  char lenstr[16];

  /* add 404 status */
  resp->status = malloc(14);
  memset(resp->status, '\0', 14);
  strncpy(resp->status, "404 Not Found", 13);

  /* add HTML */
  *body = malloc(80);
  memset(*body, '\0', 80);
  strncpy(*body, "<html><head><title>404</title></head>"
          "<body><h1>404 Not Found</h1></body></html>", 79);
  bodysize = strlen(*body);

  /* add MIME type header */
  resp_addheader(resp, "Content-Type", "text/html; charset=us-ascii");

  /* add length header */
  memset(lenstr, '\0', sizeof(lenstr));
  sprintf(lenstr, "%d", bodysize);
  resp_addheader(resp, "Content-Length", lenstr);

  return bodysize;
}
