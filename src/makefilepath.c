#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "file.h"

void makefilepath(const char *file, char **fullpath)
{
  char *pwd = malloc(MAXFILENAME);

  if (!getcwd(pwd, MAXFILENAME))
    perror("getcwd");
  else {
    int len = strlen(pwd) + strlen(file);
    *fullpath = malloc(len + 1);
    memset(*fullpath, '\0', len + 1);
    strncpy(*fullpath, pwd, strlen(pwd));
    strncat(*fullpath, file, strlen(file));
  }
}
