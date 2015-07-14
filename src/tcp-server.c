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

/**
 * After a process' death there are some remnant waiting around for the
 * parent process to pickup. By 'wait()'ing we can vanish all the ramnant.
 *
 * We could just ignore (`signal(SIGCHLD, SIG_IGN)`) making the child process
 * to be market as <defunct> in the `ps` listing and then let the OS take care
 * of it when the parent dies. But, as we prefer explicit of implicit, here i'm
 * waiting.
 *
 * `wait()` will wait for whichever child happens to exit first. If we'd like
 * to wait for a particular child, `waitpid()`.
 */
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

  // the signal handler function (or SIG_IGN to igonore)
  sa.sa_handler = &sigchld_handler;
  // set of signals to block while this one is being handled.
  // In our case, an empty set will be blocked (none)
  sigemptyset(&sa.sa_mask);
  // flags to modify the behavior of the handler (or 0).
  sa.sa_flags = SA_RESTART;

  CHKERR(
    // int sigaction(int sig, const struct sigaction *act,
    //                struct sigaction *oact)
    // sig: which signal to catch. A symbolic name from 'signal.h'
    // act: points to a struct sigaction which has fields to control the
    //      behavior of the signal handler (a pointer to a func in the struct).
    // oact: pointer to the old signal handler info that was in place before.
    //       Useful if we wish to restore the previous signal handler at a
    //       later time.
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

    printf("Connected to: %s\n", inet_ntoa(client_addr.sin_addr));

    if (!(pid = fork())) { // child
      close(s_sock_fd);

      recv(c_sock_fd, buffer, BUFSIZE, 0);
      printf("%s\n", buffer);

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

  // note that 'close' just closes OUR INTERFACE to
  // the socket and not the socket it self. When to close
  // the socket is up to the kernel. It is normal for the
  // socket to go into a TIME_WAIT state, for example.
  close(s_sock_fd);

  return EXIT_SUCCESS;
}
