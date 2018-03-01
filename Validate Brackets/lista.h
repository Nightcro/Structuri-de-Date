#include <stdlib.h>

//Program realizat de Mindru Alexandru-Ionut
//Grupa 311CB

#ifndef _LISTA_GENERICA_
#define _LISTA_GENERICA_

typedef struct celula
{
	void *info; 
	struct celula *urm;
} TCelulaG, *TLG,  **ALG;

#endif

TLG AlocCelula(void *info);
void DistLS(ALG temp);
void DistCelula(ALG temp);