#ifndef HTML_H
#define HTML_H

#include <time.h>

#define MAXREQSIZE 1024
#define MAXURISIZE 64

/* This will probably need extending */
enum httpmethod {GET};

/* HTTP request. */
struct request {
  enum httpmethod method;
  char uri[MAXURISIZE];
  char version[9];
};

/* HTTP response. */
struct response {
  char version[9];
  char *status;
  /* Header fields */
  /*struct {
    char *type;
    char *charset;
  } contenttype;
  struct tm date;
  */
  char *body;
};

/* Parse a message and, if it is a HTML request, put the data into 'req' and
   return 1, to indicate success. If the message could not be parsed properly,
   it (hopefully) isn't a properly formatted HTML request and the function will
   return 0. */
int parserequest(struct request* req, char *data, int size);

/* Convert a response structure to a string that is a valid HTTP response
   message. */
int resptostr(struct response *resp, char **buffer);

#endif /* HTML_H */
