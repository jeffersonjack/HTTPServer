#include <stdio.h>

#include "http.h"

void printrequest(struct request *req)
{
  printf("\nHTTP REQUEST\n------------\n");
  printf("Method: ");
  if (req->method == GET)
    printf("GET\n");
  else
    printf("unknown\n");
  printf("URI: %s\n", req->uri);
  printf("Version: %s\n", req->version);
  printf("\n");
}

void printresponse(struct response *resp)
{
  printf("HTTP Response\n-------------------\n");
  printf("Version: %s\n", resp->version);
  printf("Status: %s\n", resp->status);

  struct httpheader *head = resp->header;
  while (head) {
    printf("%s: %s\n", head->field, head->value);
    head = head->next;
  }
}
