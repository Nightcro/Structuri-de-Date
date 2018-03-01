#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "trie.h"

#define MAXCOPII 36

ANod createCopil(char litera, ANod nodCurent)
{
  nodCurent = (ANod)calloc(1, sizeof(TNod));
  nodCurent->Copii = calloc(MAXCOPII, sizeof(TNod));
  nodCurent->parinte = NULL;

  int i;

  for (i = 0; i < MAXCOPII; i++)
  {
    nodCurent->Copii[i] = NULL;
  }

  nodCurent->nrCopii = 0;
  nodCurent->caracterCurent = litera;
  nodCurent->FlagFinal = 0;
  nodCurent->nivel = 0;
    
  return nodCurent;
}

ANod removeCopil(char litera, ANod nodCurent)
{
  if (nodCurent->Copii != NULL)
  {
    free(nodCurent->Copii);
    nodCurent->Copii = NULL;
  }

  if (nodCurent != NULL)
  {
    //free(nodCurent);
    //nodCurent = NULL;
  }

  return nodCurent;
}

void removeCopii(ANod nod)
{
  int i;

  for (i = 0; i < MAXCOPII; i++)
  {
    if (nod->Copii[i] != NULL)
    {
      if (nod->Copii[i]->FlagFinal == 1 && nod->Copii[i]->nrCopii == 0)
      {
        nod->Copii[i] = removeCopil(nod->Copii[i]->caracterCurent, nod->Copii[i]);
      }
      else
      {
        removeCopii(nod->Copii[i]);
      }
    }
  }
}

int get_index(char litera)
{
  int index = 0;
  
  if (litera >= '0' && litera <= '9')
  {
    index = litera - '0';
  }
  else
  {
    index = litera - 'a' + '9' - '0' + 1;
  }

  return index;
}

void print_trie(FILE *file, ANod nod, char cuvantCurent[], char *aux, char pref[])
{
  int i;

  if (nod->parinte == NULL)
  {
    pref[0] = '\0'; 
  }

  aux[0] = nod->caracterCurent;
  strncat(cuvantCurent, aux, 1);

  if (nod->FlagFinal == 1)
  {
    strcat(pref, cuvantCurent);
    strcpy(cuvantCurent, pref);
    fprintf(file, "%s ", cuvantCurent);
    strcpy(pref, cuvantCurent);
    cuvantCurent[0] = '\0';
  }

  if (nod->nrCopii == 0)
  { 
    //printf("!!\n");
    fprintf(file, "%s", cuvantCurent);
  }
  else
  { 
    for (i = 0; i < MAXCOPII; i++)
    {
      if (nod->Copii[i] != NULL)
      {
        //printf("hm\n");
        pref[nod->nivel + 1] = '\0';
        print_trie(file, nod->Copii[i], cuvantCurent, aux, pref);
      }  
    }    
  }
}

void suma_trie(FILE *file, ANod nod, int *suma, int *nrCuv)
{
  int i;

  if (nod->FlagFinal == 1)
  {
    *suma = *suma + nod->nivel + 1;
    //printf("%d ", *suma);
    (*nrCuv)++;
    //printf("%d\n", *nrCuv);
  }

  if (nod->nrCopii != 0)
  {
    for (i = 0; i < MAXCOPII; i++)
    {
      if (nod->Copii[i] != NULL)
      {
        suma_trie(file, nod->Copii[i], suma, nrCuv);
      }  
    }    
  }
}