#ifndef _LISTRANK_HH_
#define _LISTRANK_HH_

#include <stdio.h>

#define NIL -1

void initRandomList (long* arr, size_t n);

long seqFindHead(const long* next, size_t n);

void seqListRanks (long head, const long* next, long* rank, size_t n);

void printList(FILE *file, long* arr, long head);

#endif 
