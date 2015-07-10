#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
  fprintf(stdout, "starting\n");
  int counter = 0;
  pid_t pid = fork();
  int i = 0;

  if (pid == 0) {
    for (; i < 100; ++i)
      fprintf(stdout, "\tchild: counter=%d\n", ++counter);
  } else if (pid > 0) {
    for (; i < 100; ++i)
      fprintf(stdout, "\t\tparent: counter=%d\n", ++counter);
  } else {
    fprintf(stderr, "Fork failed!\n");
    return 1;
  }

  fprintf(stdout, "ending\n");
  return 0;
}
