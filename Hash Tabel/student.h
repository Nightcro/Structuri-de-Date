#include <fstream>
#ifndef _STUDENT_H_
#define _STUDENT_H_

//Program realizat de Mindru Alexandru-Ionut
//Grupa 311CB

typedef struct
{
	char *nume;
	float medie;
	char grupa[6]; // 324CB + NULL
	int varsta;
} TStudent;

#endif

TStudent* readStud(FILE* in);
void printStud(void* temp, FILE* out);
void freeStud(void* temp);