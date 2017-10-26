#ifndef HTML_H
#define HTML_H

#define MAXREQSIZE    1024
#define MAXURISIZE    64
#define VERSIONLENGTH 9

enum httpmethod {GET};

struct httpheader {
  char *field;
  char *value;
  struct httpheader *next;
};

/* HTTP request. */
struct request {
  enum httpmethod method;
  char uri[MAXURISIZE];
  char version[VERSIONLENGTH];
};

/* HTTP response. */
struct response {
  char version[VERSIONLENGTH];
  char *status;
  struct httpheader *header;
};

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

#endif /* HTML_H */
