#include <sys/stat.h>

#include "file.h"

int directory(const char *path)
{
  struct stat finfo;

  if (stat(path, &finfo))
    return -1;    /* error: the file probably doesn't exist */

  if (S_ISDIR(finfo.st_mode))
    return 1;     /* directory */
  else
    return 0;     /* not a directory */
}
