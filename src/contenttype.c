#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "file.h"

int contenttype(const char *path, char *contenttype, int bsize)
{
  FILE *pipe;
  char output[MAXFILENAME], *token, command[MAXFILENAME + 8];

  memset(command, '\0', 64);
  strncpy(command, "file -i ", 8);
  strncat(command, path, strlen(path));

  pipe = popen(command, "r");
  if (pipe == NULL)
    return -1;    /* fail */

  fgets(output, bsize, pipe);

  /* get the part of the string after the first space */
  token = strtok(output, " ");
  if (strncmp(token, path, strlen(path)) != 0) {
    fprintf(stderr, "Error: output of `file -i` not as expected.\n");
    return -1;    /* fail */
  }
  token = strtok(NULL, "\n");

  if (strlen(token) >= bsize) {
    fprintf(stderr, "contenttype: buffer not of sufficient length\n");
    return -1;
  }

  /* copy the result and return success */
  memset(contenttype, '\0', bsize);
  strncpy(contenttype, token, strlen(token));

  pclose(pipe);

  return 0;
}
