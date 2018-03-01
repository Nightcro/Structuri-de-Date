#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"
#include "queue.h"
#include "lista.h"

//Program realizat de Mindru Alexandru-Ionut
//Grupa 311CB

typedef void (*print)(void*, FILE*);
typedef void (*dist)(void*);

typedef struct
{
	int id;
	char type;
} Bracket;

typedef struct
{
	void *val;	// valoarea de tip Bracket
	dist fr;	// functie de eliberare a valorii
} Info;

void *AlocBucket(size_t len, size_t dim, size_t dim_info);
int ReadVal(void *temp, FILE *in, char *op);
Info *AlocInfo(void *elem, size_t dim);
void PrintVal(void *val, FILE *out);
int cmp_brackets_id(void *a, void *b);
int cmp_brackets_type(void *a, void *b);