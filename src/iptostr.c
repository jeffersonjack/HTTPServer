#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

char * iptostr(struct sockaddr_in6 *addr)
{
  char straddr[INET6_ADDRSTRLEN];

  inet_ntop(addr->sin6_family, &(addr->sin6_addr), straddr, INET6_ADDRSTRLEN);
  //  snprintf(buffer, INET6_ADDRSTRLEN, "%s", straddr);
  
  return strdup(straddr);
}
