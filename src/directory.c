#include <sys/stat.h>
#include <string.h>

#include "file.h"

int directory(const char *path)
{
  struct stat finfo;

  if (stat(path, &finfo))
    return -1;             /* assume file doesn't exist */

  if (S_ISDIR(finfo.st_mode))
    if (*(path + strlen(path)-1) == '/')
      return DIR_OK;      /* directory */
    else
      return DIR_NOSLASH;
  else
    return 0;             /* not a directory */
}
