#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
  pid_t pid;
  // even though this LOOKS LIKE a shared variable, there's no such thing when
  // forking processes. This will be copied, not referenced. Each process has
  // its OWN COPY of ALL variables.
  int rv;

  switch(pid = fork()) {
    case -1:
      perror("fork");
      exit(EXIT_FAILURE);
    case 0:
      printf("\tCHILD: This is the child process!\n");
      printf("\tCHILD: My PID is %d\n", getpid());
      printf("\tCHILD: My parents PID is %d\n", getppid());
      printf("\tCHILD: Enter my exit status (small):\n");
      scanf("%d", &rv);
      printf("\tCHILD: I'm outta here!\n");
      exit(rv);
    default:
      printf("\tPARENT: I'm the parent process!\n");
      printf("\tPARENT: My PID is %d\n", getpid());
      printf("\tPARENT: My child's PID is %d\n", pid);
      printf("\tPARENT: I'm now waiting for my child to exit() ... \n");
      // waits on a child of the current process. The pointer to an int
      // indicates where to store the result of the child process.
      wait(&rv);
      printf("\tPARENT: My child exitted with: %d\n", WEXITSTATUS(rv));
      printf("\tPARENT: I'm outta here!\n");
  }

  return 0;
}

