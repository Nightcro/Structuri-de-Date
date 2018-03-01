#include <stdio.h>
#include <string.h>
#include "lista.h"

//Program realizat de Mindru Alexandru-Ionut
//Grupa 311CB

typedef void (*print)(void*, FILE*);
typedef int (*cmp)(void*, void*);

#ifndef _STACK_
#define _STACK_

typedef struct
{
	TLG lista;
} Stack;

typedef struct
{
	size_t len;
	Stack *bucket;
	print prt;
} TStack;

#endif

TStack *initS(size_t len, print prt);
Stack *AlocBucketS(size_t len);
int Rastoarna(Stack *temp, Stack *backward);
int top(Stack *temp, void **aux);
int push(Stack *temp, void *info);
int pop(Stack *temp, void **info);
int prints(TStack *temp, FILE *out);
int sorts(Stack *temp, cmp cmp_id, unsigned int error);
unsigned int corrects(Stack *temp, cmp cmp_type, int *ans);
void DistS(TStack **temp);