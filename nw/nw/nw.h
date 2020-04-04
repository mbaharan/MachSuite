#include <stdio.h>
#include <stdlib.h>
#include "support.h"

#define ALEN 128
#define BLEN 128

#pragma SDS data zero_copy(          \
    M [0:(ALEN + 1) * (BLEN + 1)],   \
    ptr [0:(ALEN + 1) * (BLEN + 1)], \
    SEQA [0:ALEN],                   \
    SEQB [0:BLEN],                   \
    alignedA [0:(ALEN + BLEN)],      \
    alignedB [0:(ALEN + BLEN)]))

#pragma SDS data access_pattern( \
    M                            \
    : SEQUENTIAL,                \
      ptr                        \
    : SEQUENTIAL,                \
      SEQA                       \
    : SEQUENTIAL,                \
      SEQB                       \
    : SEQUENTIAL,                \
      alignedA                   \
    : SEQUENTIAL,                \
      alignedB                   \
    : SEQUENTIAL)

void needwun(char SEQA[ALEN], char SEQB[BLEN],
             char alignedA[ALEN + BLEN], char alignedB[ALEN + BLEN],
             int M[(ALEN + 1) * (BLEN + 1)], char ptr[(ALEN + 1) * (BLEN + 1)]);

////////////////////////////////////////////////////////////////////////////////
// Test harness interface code.

struct bench_args_t
{
  char seqA[ALEN];
  char seqB[BLEN];
  char alignedA[ALEN + BLEN];
  char alignedB[ALEN + BLEN];
  int M[(ALEN + 1) * (BLEN + 1)];
  char ptr[(ALEN + 1) * (BLEN + 1)];
};
