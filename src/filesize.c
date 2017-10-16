#include <sys/stat.h>

#include "file.h"

int filesize(const char *path)
{
  struct stat finfo;

  if (stat(path, &finfo))
    return -1;    /* error: the file probably doesn't exist */

  return finfo.st_size;
}
