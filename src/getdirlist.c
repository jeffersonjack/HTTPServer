#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>

#include "file.h"

int getdirlist(const char *path, char **buffer, int bsize)
{
  DIR *dp;
  struct dirent *entry;
  char *line;
  char *tmp;
  int len = 0;

  if (*buffer) {
    /* open the directory */
    if ((dp = opendir(path))) {
      len = strlen(*buffer);
      line = malloc(MAXFILENAME);

      /* iterate through entries */
      while ((entry = readdir(dp))) {
        /* ignore hidden files (and current/parent dir) */
        if (*(entry->d_name) != '.') {
          /* build hyperlink */
          memset(line, '\0', MAXFILENAME);
          strncpy(line, "\t<li><a href=\"", 14);
          strncat(line, entry->d_name, strlen(entry->d_name));
          strncat(line, "\" >", 3);
          strncat(line, entry->d_name, strlen(entry->d_name));
          strncat(line, "</a></li>\n", 11);
          len += strlen(line);
          if (len > bsize) {
            /* buffer not big enough, extend it */
            bsize = len + MAXFILENAME;
            tmp = realloc(*buffer, bsize);
            if (tmp) {
              *buffer = tmp;
              /* initialise new part of memory */
              memset(*buffer + len, '\0', bsize - len);
            }
            else {
              perror("realloc");
              return 0;            /* fail */
            }
          }
          /* append entry to list */
          strncat(*buffer, line, strlen(line));
        }
      }
    }
  }

  return len;
}
