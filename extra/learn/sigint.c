#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

// if a signal interrupts the execution of an unsafe function and handler
// calls un unsafe function, then the behavior of the program is undefined.
//
// We must be careful since processing elsewhere may be interrupted at some
// arbitrary point in the execution. We might also call signals as a software
// interrupt. When a signal that is being caughted is handled by a process,
// the normal sequence of instructions being executed by the process is
// temporarily interrupted by the signal handler. The process then continues
// executing, but the instructions in the signal handler are now executed.
//
//    Reentrancy: a subroutine is called reentrant if it can be interrupted
//                in the middle of its execution and then safelly called again
//                (re-entered) before its previous invocations complete
//                execution. Interruptions happen by an internal action such
//                as jump or call or by an external action such as a hardware
//                interrupt of signal. After the reentered invocation
//                completes, the previous invocations will resume correct
//                execution.
void sigint_handler(int sig)
{
  // note that we're not using `printf()` here.
  write(0, "Ahhh! SIGINT!\n", 14);
}

int main(void)
{
  char s[200];
  struct sigaction sa;

  sa.sa_handler = &sigint_handler;
  sa.sa_flags = 0;
  sigemptyset(&sa.sa_mask);

  // each signal has a current disposition which determines how the process
  // behaves when it is delivered the signal. `sigaction()` changes it.
  // The disposition is a per-process attribute so that in a multithread env
  // the disposition of a particular signal is the same for all threads.
  //
  // A child create via fork() inherits a copy of its parent's signal
  // dispositions.
  if (!~sigaction(SIGINT, &sa, NULL)) {
    perror("sigaction");
    exit(EXIT_FAILURE);
  }

  printf("Enter a string:\n");

  if (fgets(s, sizeof s, stdin) == NULL)
    perror("fgets");
  else
    printf("You entered: %s\n", s);

  exit(EXIT_SUCCESS);
}
