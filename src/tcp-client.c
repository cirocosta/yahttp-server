#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>

#include "CHK.h"

int main(int argc, char *argv[])
{
  struct hostent *h;

  if (argc != 2) {
    fprintf(stderr, "usage: getip address\n");
    exit(EXIT_FAILURE);
  }

  h = gethostbyname(argv[1]);
  CHKNULL_H(h, "gethostbyname:");

  printf("Hostname:\t %s\n", h->h_name);
  printf("IP Address:\t %s\n", inet_ntoa(*((struct in_addr*)h->h_addr_list[0])));

  return 0;
}
