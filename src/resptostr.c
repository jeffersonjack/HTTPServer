#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "http.h"

int resptostr(struct response *resp, char **buffer)
{
  int size = 0;

  /* status line */
  size += strlen(resp->version);
  size += 1;    /* space */
  size += strlen(resp->status);
  size += 2;    /* <CR><LF> */
  /* TODO: header fields */
  size += 2;    /* blank line */

  /* allocate and initialise buffer */
  *buffer = NULL;
  *buffer = malloc(size+1); /* +1 for null-terminator */
  if (*buffer == NULL) {
    perror("malloc");
    return -1;
  }
  memset(*buffer, '\0', size);

  /* build response string */
  strncpy(*buffer, resp->version, strlen(resp->version));
  strncat(*buffer, " ", 1);
  strncat(*buffer, resp->status, strlen(resp->status));
  strncat(*buffer, "\r\n", 2);    /* end status line */
  strncat(*buffer, "\r\n", 2);    /* blank line */

  return size;
}
