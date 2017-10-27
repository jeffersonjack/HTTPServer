#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "http.h"
#include "file.h"

int makeresponse_file(struct response *resp, const char *path, char **contents)
{
  int filesize;
  FILE *f;
  struct stat finfo;
  char buffer[1024];
  char timestr[TIMESTRLEN];

  if (stat(path, &finfo)) {
    fprintf(stderr, "stat: error getting file info\n");
    return -1;
  }

  /* get size of file (bytes) */
  filesize = finfo.st_size;

  /* allocate enough space to store the file */
  *contents = malloc(filesize);

  /* open the file, in binary mode, for reading */
  if ((f = fopen(path, "rb")) == NULL) {
    perror("fopen");
    return -1;
  }

  if (fread(*contents, sizeof(char), filesize, f) != filesize) {
    fprintf(stderr, "fread: wrong number of bytes was read\n");
    return -1;
  }

  /* succesfully read the file, close it... */
  fclose(f);

  /* ...and indicate success in HTTP status field */
  resp->status = malloc(7);
  memset(resp->status, '\0', 7);
  strncpy(resp->status, "200 OK", 6);

  /* convert length to a string and add it as a header field */
  memset(buffer, '\0', sizeof(buffer));
  sprintf(buffer, "%d", filesize);
  resp_addheader(resp, "Content-Length", buffer);

  /* try and get the MIME type of the file, and add it as a header */
  memset(buffer, '\0', sizeof(buffer));
  if (contenttype(path, buffer, sizeof(buffer)) == 0)
    resp_addheader(resp, "Content-Type", buffer);

  /* add last modification time header */
  memset(timestr, '\0', TIMESTRLEN);
  httptime(&(finfo.st_mtim.tv_sec), timestr);
  resp_addheader(resp, "Last-Modified", timestr);

  /* success */
  return filesize;
}
