#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <wait.h>
#include <unistd.h>

/**
 * pipe() creates a unidirectional data channel that we can use for IPC.
 * We pass an array of 2 ints that will contain the file descriptors
 * referring to the ends of the pipe (read and write ends, respectively).
 *
 * Data written to the write end if buffered by the kernel until it is read
 * from the read end of the pipe.
 *
 * If a process attempts to read from an empty pipe, then read() will block
 * until data is available. If a process attempts to write to a full pipe,
 * then write() blocks until sufficient data has been read from the pipe
 * to allow the write to complete. Using `fcntl` one can define a
 * nonblocking IO configuration.
 *
 * AN APPLICATION SHOULD NOT RELY A PARTICULAR PIPE CAPACITY: it should be
 * designed so that a reading process consumes data as soon as it's
 * available, so that a writing process does not remain blocked.
 */
int main(int argc, char *argv[])
{
  int pfds[2];
  char buf[30];
  pid_t pid;

  if (!~pipe(pfds)) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  if (!(pid = fork())) {
    printf("\tCHILD: writing to the pipe\n");
    write(pfds[1], "test", 5);
    printf("\tCHILD: exiting\n");
    exit(EXIT_SUCCESS);
  } else if (pid > 0){
    printf("PARENT: reading from pipe\n");
    read(pfds[0], buf, 5);
    printf("PARENT: read: '%s'\n", buf);
    wait(NULL);
  } else
    perror("fork");

  exit(EXIT_SUCCESS);
}
