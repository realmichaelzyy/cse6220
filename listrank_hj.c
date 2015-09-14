#include <stdlib.h>
#include <unistd.h>
#include <omp.h>
#include <math.h>
#include "listutils.h"
using namespace std;


//generate n random numbers with max limit of "max" and not equal to head
void genRandomList(long* a, size_t n, long max, long head)
{
  long x;
  bool exist=0;

  for (long i=0; i<n; i++)
  {
     x= rand() % max;

     //search for x to see whether it already exisits. to be paralized
     if ( x==head ) exist=1;
     for (long j=0; j<i; j++) 
     {
       if ( x==a[j] )
       {
          exist=1;
          break;
       }
     }
     if (exist==0)
       a[i]=x;
     else 
     {
       i--;       
       exist=0;
     }
  }
}


void
parallelListRanks (long head, const long* next, long* rank, size_t n)
{
  long* heads; //array to save all the head nodes
  long* subsize; //array to save all the previous subarray's size
  long* headnext; //stores the next head of each subhead, so wo can know the sublist orders
  long s; //records the number of sublists
  int p; //number of processors

/*
  printf("\nglobal head: %ld\n",head);
  printf("\ninitial next array:\n");
  for( long i=0; i<n; i++)
  {
     printf(" %ld ", next[i]);
  }
*/

  //compute number of sublists
  p = omp_get_num_procs();   
  s= p*ceil(log(n)/log(2));
//  s=3;

  //generate s-1 random sub head nodes
//*************************************************************
  heads = (long*) malloc(s * sizeof(long));
  genRandomList(heads, s-1, n, head);
  heads[s-1]=head;  //last element in heads[] is the global head
  subsize = (long*) malloc(s * sizeof(long));
  headnext = (long*) malloc(s * sizeof(long));
/*
  printf("\nnumber of processors:%d ; number of list:%ld ; number of sublists:%ld\n ", p,n,s);  
  printf("random picked sublist heads:\n");
  for (long i=0; i<s; i++)
   printf(" %ld ", heads[i]);
  printf("\n");
*/
  //set the head nodes rank to NIL in the rank array, and the headnext array to NIL
  #pragma omp parallel
  {
     #pragma omp for
     for (long i=0; i<s; i++)
     {
        rank[heads[i]]=NIL;
        headnext[i]=NIL;
     }
  }

  //compute the ranks of each sublists. //try dynamic schedule
//**************************************************************** 
 #pragma omp parallel
  {
     #pragma omp for schedule (dynamic)
     for (long i=0; i<s; i++)
     {
       //  printf("\nsublist %ld:",i);
         long subpointer = next[heads[i]];
         long r = 1; 
       //  printf(" nexthead=%ld\n", subpointer);
         while ( subpointer != NIL )
         {
            if (rank[subpointer] == NIL) // reach the next sublist head 
            {
                //search the next subhead position in the headlist, and store it to the headnext array
                int j=0;
                for (j=0; j<s; j++)
                {
                   if (subpointer==heads[j])
                   {  headnext[i]=j;
                      subsize[j]=r;
                      break;  }
                }
                if (j==s) printf("\nerror!sublist head not found!"); 
                break;
            } 
            rank[subpointer] = r++;
            subpointer = next[subpointer];
         //   printf("+1! ");
         }
     }
  } 
/*
  printf("\nrank after subranking:\n");
  for (long i=0; i<n; i++)
  {
     printf(" %ld ", rank[i]);
  }
  printf("\nsubsize count:\n");
  for (long i=0; i<s; i++)
  {
     if (headnext[i] != NIL) {
        printf("sublist %ld has %ld elements",i,subsize[headnext[i]]);
        printf("\nnext array head is:%ld\n",heads[headnext[i]]);
     }
  }
*/
  //calculate the prefix sum of the subsize array
//*******************************************************
  subsize[s-1] = 0;
  long r;
  long subpointer = s-1; 
  rank[head]=0;
  while(headnext[subpointer] != NIL)
  {
    subsize[headnext[subpointer]]+=subsize[subpointer]; 
    subpointer = headnext [subpointer];
    rank[heads[subpointer]]=subsize[subpointer];  //write back the sublist head's rank
  } 
/*
  for(long i=0; i<s; i++)
  {
     printf("sublist head %ld's rank = %ld\n", heads[i], subsize[i]);
  } 
*/

//parallel add back each subhead's rank to each elements in the sublist's rank
//*********************************************************
 #pragma omp parallel
  {
     #pragma omp for schedule (dynamic)
     for (long i=0; i<s; i++)
     {
         long subpointer = next[heads[i]];
         while ( subpointer != NIL )
         {
             if (subpointer == heads[headnext[i]]) // reach the next sublist head
             {break;} 
             rank[subpointer]+=subsize[i];
             subpointer = next[subpointer];
         }
     }
  }
/*
  printf("\nfinal rank:\n");
  for (int i=0; i<n; i++)
  {
    printf(" %ld ", rank[i]);
  }
  printf("\n");
*/

//deconstruct
//**********************************************************
  free(heads);
  free(subsize);
  free(headnext);



/*  long r = 0;

  while(head != NIL){
    rank[head] = r++;
    head = next[head];
  }
*/
}
