#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


/**
 * Performs:
 *
 * ls | wc - l
 *
 * i.e, connects the first exec's stdout to the second's stdin though
 * a pipe. Remember: pipe() gives us two file descriptors - one to read from
 * (the first) and another to write to (the second).
 *
 * `dup(oldfd)` is the responsible for creating a copy of the filedescriptor
 * `oldfd`. The lowest-mumbered unused descriptor is then used for the new
 *  descriptor.
 *
 * Pay attention that:
 * -  The first exec will write to the pipe. The second will read from.
 * -  The stdout of the first exec must be the pipe's write end.
 * -  The stdin of the second exec must be the pipe's read end.
 *
 * And remember the default fds:
 *  0 --> stdin
 *  1 --> stdout
 *  2 --> stderr
 */
int main(int argc, char *argv[])
{
  // 0 : read end
  // 1 : write end
  int pfds[2];

  if (!~pipe(pfds)) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  if (!fork()) {                    // parent
    close(1);
    dup(pfds[1]);
    close(pfds[0]);
    execlp("ls", "ls", NULL);
  } else {                          // child
    close(0);
    dup(pfds[0]);
    close(pfds[1]);
    execlp("wc", "wc", "-l", NULL);
  }

  exit(EXIT_SUCCESS);
}
