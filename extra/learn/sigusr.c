/**
 * Ps: note that `sig_atomic_t` has NOTHING to do with the multithread
 *     aspect of an application. It's only relevant for ensuring atomicity
 *     of operations interrupted by a signal handler *IN THE SAME THREAD*.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

volatile sig_atomic_t got_usr1 = 0;

void sigusr1_handler(int sig)
{
  got_usr1 = 1;
}

/**
 * This example illustrate signalling from anywhere else by using a custom
 * signal.
 *
 * Run (`$ ./sigusr.out`) and then send the signal in the terminal:
 * `$ kill -USR1 PID`
 */
int main(void)
{
  struct sigaction sa;
  sa.sa_handler = &sigusr1_handler;
  sa.sa_flags = 0;
  sigemptyset(&sa.sa_mask);

  if (!~sigaction(SIGUSR1, &sa, NULL)) {
    perror("sigaction");
    exit(EXIT_FAILURE);
  }

  while (!got_usr1) {
    printf("PID %d: working hard ... \n", getpid());
    sleep(1);
  }

  printf("Done in SIGUSER1!\n");

  exit(EXIT_SUCCESS);
}

