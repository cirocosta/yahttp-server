/**
 * With pipe() one process cannot just grab one end of a normal pipe that
 * was created by an unrelated process. With FIFOs each unrelated process
 * can simply open() the pipe and transfer data through it.
 *
 * FIFOs are actuall files on disk. To create these special files we must
 * create them with `mknod()`, which creates a filesystem node (file, device
 * special file or named pipe).
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define FIFO_NAME "american_maid"

int main(int argc, char *argv[])
{
  char s[300];
  int num, fd;

  mknod(FIFO_NAME, S_IFIFO | 0666, 0);

  printf("waiting for reader ... \n");
  // will block until someone else opens it to read from it.
  fd = open(FIFO_NAME, O_WRONLY);
  printf("got a reader -- type some stuff\n");

  while (fgets(s, 300, stdin)) {
    if ((num = write(fd, s, strlen(s))) == -1)
      perror("write");
    else
      printf("speak: wrote %d bytes\n", num);
  }

  exit(EXIT_SUCCESS);
}

