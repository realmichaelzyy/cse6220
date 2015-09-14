#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include "listutils.h"
using namespace std;

void jumpList (long* nin, long* nout, size_t n)
{
/*
  printf("\n\nnin before jump\n");
  for (int i=0; i<n; i++)
  {
     printf(" %ld ", nin[i]);
  }
  printf("\n");
*/

  #pragma omp parallel
  {
     #pragma omp for
     for ( int i=0; i<n; i++)
     {
       if (nin[i] != NIL)
          nout[i] = nin[nin[i]];
//     else nout[i]= NIL;
     }
  } 
} 

void updateRanks(long* rin, long* rout, long* next, size_t n) 
{
  #pragma omp parallel
  {
     #pragma omp for
     for ( int i=0; i<n; i++)
     {
        if (next[i] != NIL)
           rout[next[i]]=rin[i]+rin[next[i]];
     }
  }
}

//assign a2 to a1. a1=a2
void copyarray(long* a1, long* a2, size_t n)
{
  #pragma omp parallel
  {
     #pragma omp for
     for ( int i=0; i<n; i++ )
     {
       a1[i] = a2[i];
     }
  }
}
 
/*
void swap(long* a1, long* a2, size_t n)
{
  long *atemp = (long*) malloc(n * sizeof(long));
  #pragma omp parallel
  {
     #pragma omp for
     for ( int i=0; i<n; i++ )
     {
       atemp[i] = a1[i];
       a1[i] = a2[i];
       a2[i] = atemp[i];
     }
  }
  free (atemp);
} 
*/

void
parallelListRanks (long head, const long* next, long* rank, size_t n){
  /* Replace the code below with your implementation of the Wyllie algorithm */
//  printf("\nhello, Chang, How are you doing?\n");
  long r = 0;
  long depth = ceil(log(n)/log(2));
//  printf("size: %ld  depth: %ld\n",n,depth);
 
     long *r2 = (long*) malloc(n * sizeof(long));
     long *n1 = (long*) malloc(n * sizeof(long));
     long *n2 = (long*) malloc(n * sizeof(long));  

  #pragma omp parallel
  {
     #pragma omp for 
     for (int i=0; i<n; i++)
     {
       rank[i]=1;
       r2[i]=1;
       n1[i]=next[i];
       n2[i]=next[i];
     }
     rank[head]=0;
     r2[head]=0;
  }

/*
  //print the initial next array and head pointer
  printf("Initial status:\nnext array:\n");
  for (int i=0; i<n; i++)
  {
    printf(" %ld ", next[i]);
  }
  printf("\nhead: %ld\n",head);
  printf("rank:\n");
  for (int i=0; i<n; i++)
  {
     printf(" %ld ", rank[i]);
  }
  printf("\n\n");
*/

  for (int i=0; i<depth; i++)
  {
     updateRanks(rank, r2, n1, n); //push
     jumpList(n1, n2, n);  //jump
     copyarray(rank, r2, n);
     copyarray(n1, n2, n);

/*
     // print the status after each round push and jump
     printf("\n\nround %d\nnext:\n", i);
     for(int j=0; j<n; j++)
     {
        printf(" %ld ", n1[j]);
     }
     printf("\nrank:\n");
     for (int j=0; j<n; j++)
     {
        printf(" %ld ", rank[j]);
     }
*/ 
  }

  free(r2);
  free(n1);
  free(n2);

//  while(head != NIL){
//    rank[head] = r++;
//    head = next[head];
//  }

}
