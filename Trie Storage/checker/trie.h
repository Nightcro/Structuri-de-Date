/*-- trie.h ---*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#ifndef _TRIE
#define _TRIE

typedef struct nod
{ 
  struct nod **Copii;
  struct nod *parinte;
  char caracterCurent;
  int nrCopii;
  int FlagFinal;          //1:sfarsit de cuvant
  int nivel;
} TNod, *ANod, **ATrie;


ANod createCopil(char, ANod);

ANod removeCopil(char, ANod);

void removeCopii(ANod nod);

int get_index(char);

void suma_trie(FILE *, ANod, int *, int *);

#endif