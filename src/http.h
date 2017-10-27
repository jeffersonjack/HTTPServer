#ifndef HTML_H
#define HTML_H

#include <time.h>

#define MAXREQLEN 1024
#define MAXURILEN   64
#define VERSIONLEN   9
#define TIMESTRLEN  30

enum httpmethod {GET};

struct httpheader {
  char *field;
  char *value;
  struct httpheader *next;
};

/* HTTP request. */
struct request {
  enum httpmethod method;
  char uri[MAXURILEN];
  char version[VERSIONLEN];
};

/* HTTP response. */
struct response {
  char version[VERSIONLEN];
  char *status;
  struct httpheader *header;
};

/* assuming there is a valid file at 'path', fill the response structure with
   relevant information, and put the contents into 'contents'. returns the size
   of the file on success, and -1 on error */
int makeresponse_file(struct response *resp, const char *path, char **contents);

/* assuming the file at 'path' is a directory, generate an HTTP response and a
   directory listing (in HTML format). returns the number of bytes in the
   listing on success, and -1 on error */
int makeresponse_dir(struct response *resp, const char *uri,
                     const char *path, char **listing);

/* make an HTTP response that indicates that the file requested doesn't
   exist. returns the number of bytes in the body on success, -1 on error */
int makeresponse_404(struct response *resp, char **body);

/* Add a new header onto the end of the linked list of header fields */
void resp_addheader(struct response *resp, char *field, char *value);

/* Free all of the memory that was allocated for use with header fields */
void resp_freeheaders(struct response *resp);

/* Parse a message and, if it is a HTML request, put the data into 'req' and
   return 1, to indicate success. If the message could not be parsed properly,
   it (hopefully) isn't a properly formatted HTML request and the function will
   return 0. */
int parserequest(struct request* req, char *data, int size);

/* Convert a response structure to a string that is a valid HTTP response
   message. */
int resptostr(struct response *resp, char **buffer);

/* print an HTTP request message */
void printrequest(struct request *req);

/* print an HTTP response message */
void printresponse(struct response *resp);

/* get the date/time specified by 'secs' (since the Epoch) in HTTP format
   (specified in RFC7231) */
void httptime(time_t *secs, char *buffer);

#endif /* HTML_H */
