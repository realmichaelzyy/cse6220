#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <getopt.h>

#include "listutils.h"

extern void 
parallelListRanks (long head, const long* next, long* rank, size_t n);

/* ====================================================================== */

#define USAGE                                                                 \
"usage:\n"                                                                    \
"  ./correctness [options]\n"                                                        \
"options:\n"                                                                  \
"  -t [num_trials]     Number of test trials (Default: 1)\n"                         \
"  -n [list_length]    Length of the list (Default: 100000)\n"                \
"  -h                  Show this help message\n" 

int
main (int argc, char* argv[])
{
  int option_char;
  int NTRIALS = 1;
  int N = 100000;

  if(argc == 1) fprintf(stderr, "%s\n", USAGE);

  while ((option_char = getopt(argc, argv, "t:n:h")) != -1) 
  {
    switch (option_char) 
    {
      case 't': 
        NTRIALS = atoi(optarg);
        break;
      case 'n':
        N = atoi(optarg);
        break;
      case 'h':
        fprintf(stderr, "%s\n", USAGE);
        exit(EXIT_SUCCESS);
        break;                      
      default:
        fprintf(stderr, "%s\n", USAGE);
        exit(EXIT_FAILURE);
    }
  }

  long *next = (long*) malloc(N * sizeof(long));
  long *par_rank = (long*) malloc(N * sizeof(long));
  long *seq_rank = (long*) malloc(N * sizeof(long));

  bool matching = true;
  for(int i = 0; i < NTRIALS; i++){

    initRandomList(next, N);

    long head = seqFindHead(next, N);

    seqListRanks(head, next, seq_rank, N);
    parallelListRanks(head, next, par_rank, N);

    if( memcmp(seq_rank, par_rank, N * sizeof(long)) != 0){
      matching = false;
      printf("Ranks did not match.\n");
      break;
    }
  }
  if(matching)
  {
    printf("Your implementation produces the correct list!\n");
  }
  else
  {
    printf("Your implementation did not produce the correct list!\n");
  }

  free(next);
  free(par_rank);
  free(seq_rank);

  return EXIT_SUCCESS;
}
