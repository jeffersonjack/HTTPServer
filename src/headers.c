#include <stdlib.h>
#include <string.h>

#include "http.h"

void resp_addheader(struct response *resp, char *field, char *value)
{
  int fieldlen, valuelen;
  struct httpheader *ptr, *new;

  ptr = resp->header;
  fieldlen = strlen(field);
  valuelen = strlen(value);

  /* find the end of the list */
  if (ptr) {
    while (ptr->next)
      ptr = ptr->next;
  }

  /* create a new header field */
  new = malloc(sizeof(struct httpheader));
  memset(new, 0, sizeof(struct httpheader));
  new->field = malloc(fieldlen + 1);
  memset(new->field, '\0', fieldlen + 1);
  strncpy(new->field, field, fieldlen);
  new->value = malloc(valuelen + 1);
  memset(new->value, '\0', valuelen + 1);
  strncpy(new->value, value, valuelen);

  /* add the new header field to the end of the list */
  if (ptr)
    ptr->next = new;
  else
    resp->header = new;
}

void freenode(struct httpheader *head)
{
  if (head->next) {
    freenode(head->next);
    head->next = NULL;
  }
  if (head->field) {
    free(head->field);
    head->field = NULL;
  }
  if (head->value) {
    free(head->value);
    head->value = NULL;
  }
  free(head);
}

void resp_freeheaders(struct response *resp)
{
  if (resp && resp->header) {
    freenode(resp->header);
  }
}
