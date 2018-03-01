#ifndef _MATERIE_H_
#define _MATERIE_H_

//Program realizat de Mindru Alexandru-Ionut
//Grupa 311CB

typedef struct
{
	char *materie;
	int ore_curs;
	int ore_lab;
	int credit;
	int teme;
} TMaterie;

#endif

TMaterie* readMat(FILE* in);
void printMat(void* temp, FILE* out);
void freeMat(void* temp);