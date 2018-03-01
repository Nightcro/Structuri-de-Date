#ifndef _LISTA_GENERICA_
#define _LISTA_GENERICA_

//Program realizat de Mindru Alexandru-Ionut
//Grupa 311CB

typedef void (*dist)(void*);

typedef struct celulag
{
  void *info;
  struct celulag *urm;
} TCelulaG, *TLG, **ALG;

#endif

int InsertTLG(ALG temp, void* cell, size_t dim);
void DistCelula(ALG temp);
void DistLS(ALG temp, dist fr);