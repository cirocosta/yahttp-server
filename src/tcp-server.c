#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#include "CHK.h"

#define PORT 8080
#define BACKLOG 10
#define BUFSIZE 1024

// TODO understand this better.
void sigchld_handler(int s)
{
  while(wait(NULL) > 0);
}

int main(void)
{
  int s_sock_fd, c_sock_fd;
  struct sockaddr_in server_addr;
  struct sockaddr_in client_addr;
  struct sigaction sa;

  char buffer[BUFSIZE];

  unsigned int sin_size;
  int yes=1;
  int pid;

  CHKERR(
    s_sock_fd = socket(AF_INET, SOCK_STREAM, 0),
    "socket"
  );

  CHKERR(
    setsockopt(s_sock_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)),
    "setsockopt"
  );

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = INADDR_ANY;
  memset(&(server_addr.sin_zero), '\0', 8);

  printf("Will serve at: %s:%d\n",
         inet_ntoa(server_addr.sin_addr),
         ntohs(server_addr.sin_port));
  gethostname(buffer, BUFSIZE);
  printf("Hosting from: %s\n", buffer);

  CHKERR(
    bind(s_sock_fd, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)),
    "bind"
  );

  CHKERR(
    listen(s_sock_fd, BACKLOG),
    "listen"
  );

  // TODO undeerstand this better.
  sa.sa_handler = &sigchld_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;

  CHKERR(
    sigaction(SIGCHLD, &sa, NULL),
    "sigaction"
  );

  while (1) {
    sin_size = sizeof(struct sockaddr_in);
    c_sock_fd = accept(s_sock_fd, (struct sockaddr*)&client_addr, &sin_size);

    if (!~c_sock_fd) {
      perror("accept");
      continue;
    }

    printf("Connected: %s\n", inet_ntoa(client_addr.sin_addr));

    if (!(pid = fork())) { // child
      close(s_sock_fd);
      if (!~send(c_sock_fd, "Hello, world!\n", 14, 0))
        perror("send");

      close(c_sock_fd);
      exit(EXIT_SUCCESS);
    } else if (pid > 0) { // parent
      close(c_sock_fd);
      printf("Gone: %s\n", inet_ntoa(client_addr.sin_addr));
    } else {  // error
      perror("fork()");
    }
  }

  return EXIT_SUCCESS;
}
