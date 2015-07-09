#ifndef CHK_H
#define CHK_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define CHKERR(var, msg) \
  do { \
    if (!~(var)) { \
      fputs(msg, stderr); \
      fputs(strerror(errno), stderr); \
      exit(EXIT_FAILURE); \
    } \
  } while (0)

#define CHKNULL(var, msg) \
  do { \
    if ((var) == NULL) { \
      fputs(msg, stderr); \
      fputs(strerror(errno), stderr); \
      exit(EXIT_FAILURE); \
    } \
  } while (0)

#define CHKNULL_H(var, msg) \
  do { \
    if ((var) == NULL) { \
      fputs(msg, stderr); \
      fputs(strerror(h_errno), stderr); \
      exit(EXIT_FAILURE); \
    } \
  } while (0)

#define CHKERR_H(var, msg) \
  do { \
    if (!~(var)) { \
      fputs(msg, stderr); \
      fputs(strerror(h_errno), stderr); \
      exit(EXIT_FAILURE); \
    } \
  } while (0)


#endif
