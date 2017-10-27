#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "file.h"
#include "http.h"

int makeresponse_dir(struct response *resp, const char *uri,
                     const char *path, char **listing)
{
  int listingsize;
  char lenstr[16];

  /* make page header */
  listingsize = 73 + strlen(uri);
  *listing = malloc(listingsize);
  memset(*listing, '\0', listingsize);
  strncpy(*listing, "<html><head><title>Index</title></head><body><h1>Index of ", 58);
  strncat(*listing, uri, strlen(uri));
  strncat(*listing, "</h1><hr><ul>\n", 14);

  /* get the listing */
  if ((listingsize = getdirlist(path, listing, listingsize)) == 0)
    fprintf(stderr, "getdirlist: failed to get directory listing\n");
  /* add success status */
  resp->status = malloc(7);
  memset(resp->status, '\0', 7);
  strncpy(resp->status, "200 OK", 6);
  /* add MIME type header */
  resp_addheader(resp, "Content-Type", "text/html; charset=us-ascii");
  /* add length header */
  memset(lenstr, '\0', sizeof(lenstr));
  sprintf(lenstr, "%d", listingsize);
  resp_addheader(resp, "Content-Length", lenstr);

  /* success */
  return listingsize;
}
