#include <assert.h>
#include <stdlib.h>
#include <ostream>
#include "listutils.h"

void initRandomList (long* next, size_t n)
{
  long *pi = (long*) malloc(n * sizeof(long));

  //Inside-out Fisher-Yates (Knuth) shuffle
  for (long i = 0; i < n; ++i){
    long j = rand() % (i+1);  
    pi[i] = pi[j];
    pi[j] = i;
  }

  for(long i = 0; i < n - 1; ++i)
    next[pi[i]] = pi[i+1];
  next[pi[n-1]] = NIL;

  free(pi);
}

long seqFindHead(const long* next, size_t n){
  long ans;

  int* hasPrev = (int*) malloc(n * sizeof(int));

  for(long i = 0; i < n; i++)
    hasPrev[i] = NIL;

  for(long i = 0; i < n; i++)
    if (next[i] != NIL)
      hasPrev[next[i]] = 1;
    
  for(long i = 0; i < n; i++)
    if (hasPrev[i] == NIL){
      free(hasPrev);
      return i;
    }

  assert(0);

  free(hasPrev);

  return NIL;
}

void
seqListRanks (long head, const long* next, long* rank, size_t n){
  long r = 0;

  while(head != NIL){
    rank[head] = r++;
    head = next[head];
  }
}

void 
printList(FILE *file, long* arr, long head){
  while(head != NIL){
    fprintf(file, "%ld\n", head);
    head = arr[head];
  }
}
