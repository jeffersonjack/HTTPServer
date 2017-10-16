#include <stdlib.h>
#include <string.h>

#include "file.h"

void makefilepath(const char *file, char **fullpath)
{
  int len = strlen(BASEDIR) + strlen(file) + 1;
  *fullpath = malloc(len * sizeof(char));
  memset(*fullpath, '\0', len);
  strncpy(*fullpath, BASEDIR, len);
  strncat(*fullpath, file, strlen(file));
}
