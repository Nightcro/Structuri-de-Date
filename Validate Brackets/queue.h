#include <stdio.h>
#include <string.h>
#include "lista.h"

//Program realizat de Mindru Alexandru-Ionut
//Grupa 311CB

typedef void (*print)(void*, FILE*);
typedef int (*cmp)(void*, void*);

#ifndef _QUEUE_
#define _QUEUE_

typedef struct
{
	TLG lista;
	TLG ultim;
} Queue;

typedef struct
{
	size_t len;
	Queue *bucket;
	print prt;
} TQueue;

#endif

TQueue *initQ(size_t len, print prt);
Queue *AlocBucketQ(size_t len);
int front(Queue *temp, void **aux);
int intrq(Queue *temp, void *info);
int extrq(Queue *temp, void **info);
int printq(TQueue *temp, FILE *out);
int sortq(Queue *temp, cmp cmp_id, unsigned int error);
unsigned int correctq(Queue *temp, cmp cmp_type, int *ans);
void DistQ(TQueue **temp);