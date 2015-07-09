#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <arpa/inet.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include "CHK.h"

#define PORT 8080
#define MAXDATASIZE 100

/**
 * Hostent: Description of data base entry for a single host.
 */

int main(int argc, char *argv[])
{
  int sock_fd, numbytes;
  char buf[MAXDATASIZE];
  struct hostent *he;
  struct sockaddr_in server_addr;

  if (argc != 2) {
    fprintf(stderr, "usage: tcp-client address\n");
    exit(EXIT_FAILURE);
  }

  CHKNULL_H(
    he = gethostbyname(argv[1]),
    "gethostbyname:"
  );

  printf("Connecting to:\n");
  printf("\tHostname:\t %s\n", he->h_name);
  printf("\tIP Address:\t %s:%d\n",
         inet_ntoa(*((struct in_addr*)he->h_addr_list[0])), PORT);


  CHKERR(
    sock_fd = socket(AF_INET, SOCK_STREAM, 0),
    "socket"
  );

  printf("Socket Created!\n'");

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr = *((struct in_addr *)he->h_addr_list[0]);
  memset(&(server_addr.sin_zero), '\0', 8);

  CHKERR(
    connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)),
    "connect"
  );

  printf("Connection Established!");

  CHKERR(
    numbytes = recv(sock_fd, buf, MAXDATASIZE-1, 0),
    "recv"
  );
  send(sock_fd, "hello world\n", 12, 0);

  buf[numbytes] = '\0';
  printf("Received: %s\n", buf);
  close(sock_fd);

  return 0;
}
