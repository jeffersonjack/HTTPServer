#ifndef HTML_H
#define HTML_H

#define MAXURILEN 64

/* This will probably need extending */
enum httpmethod {GET};

/* HTTP request. */
struct request {
  enum httpmethod method;
  char uri[MAXURILEN];
  char version[9];
};

/* HTTP response. */
struct response {
  char *version;
  char *status;
  /* Header fields */
  struct {
    char *type;
    char *charset;
  } contenttype ;
  char *date;
};

/* Parse a message and, if it is a HTML request, put the data into 'req' and
   return 1, to indicate success. If the message could not be parsed properly,
   it (hopefully) isn't a properly formatted HTML request and the function will
   return 0. */
int parserequest(struct request* req, char *data, int size);

#endif /* HTML_H */
