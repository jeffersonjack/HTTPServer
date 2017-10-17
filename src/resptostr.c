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

  /* header fields */
  /* TODO */

  /* body */
  size += 2;    /* blank line */
  if (resp->body)
    size += strlen(resp->body);
  size += 1;    /* null terMinator */

  /* allocate and initialise buffer */
  *buffer = NULL;
  *buffer = malloc(size);
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
  if (resp->body)
    strncat(*buffer, resp->body, strlen(resp->body));
  strncat(*buffer, "\0", 1);

  return size;
}
