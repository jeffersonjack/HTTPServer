#include <stdlib.h>
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
  size += strlen(resp->body);
  size += 1;    /* null terminator */

  *buffer = malloc(size * sizeof(char));
  if (!*buffer)
    return -1;

  strncpy(*buffer, resp->version, strlen(resp->version));
  strncat(*buffer, " ", 1);
  strncat(*buffer, resp->status, strlen(resp->status));
  strncat(*buffer, "\r\n", 2);    /* end status line */

  strncat(*buffer, "\r\n", 2);    /* blank line */
  strncat(*buffer, resp->body, strlen(resp->body));
  strncat(*buffer, "\0", 1);

  return size;
}
