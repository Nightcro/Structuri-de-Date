#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "trie.h"
#include "trie.c"

#define MAXCOPII 36


void add(ATrie trie, char word[])
{
  int i, len = strlen(word), index = 0;

  for (i = 0; i < len; i++)
  {
    index = get_index(word[i]);
    if( (*trie)->Copii[index] == NULL)
    {
      (*trie)->Copii[index] = createCopil(word[i], NULL);
      (*trie)->Copii[index]->parinte = *trie;
      (*trie)->nrCopii++;   
      if ((*trie)->parinte != NULL)
      {
        (*trie)->nivel = (*trie)->parinte->nivel + 1;
      }
    }
    trie = &(*trie)->Copii[index];
  }
  (*trie)->FlagFinal = 1;
  (*trie)->nivel = (*trie)->parinte->nivel + 1;
}

void delete(ATrie trie, char word[])
{
  int i, len = strlen(word), gasit = 0, index = 0;
  ANod nod = *trie, aux = NULL;
  
  for (i = 0; i < len; i++)
  { 
    index = get_index(word[i]);
    nod = nod->Copii[index];
    if (nod == NULL)
    { 
      gasit = 1;
      break;
    }
  }
  
  if (gasit == 0)
  {
    while (nod->caracterCurent != ' ')
    {
      if (nod->nrCopii == 0)
      {
        aux = nod;
        nod = nod->parinte;
        removeCopil(aux->caracterCurent, aux);
        nod->nrCopii--;
      }
    }
  }
}

void find(FILE *file, ATrie trie, char word[])
{    
  int i, gasit = 0, index = 0;
  ANod nod = *trie;
  
  for (i = 0; i < strlen(word); i++)
  { 
    index = get_index(word[i]);
    nod = nod->Copii[index];
    if (nod == NULL)
    { 
      gasit = 1;
      break;
    }
  }

  //printf("%c\n", nod->cuvantCurent);
  //printf("%d\n", nod->FlagFinal);
  if (nod != NULL)
  {
    if (nod->FlagFinal == 0)
    {
      fprintf(file, "False\n");
      return;
    }
  }

  if (gasit == 0)
    fprintf(file, "True\n");
  else
    fprintf(file, "False\n");
}

void find_longest_prefix(FILE *file, ATrie trie, char word[])
{ 
  int i, len = strlen(word), gasit = 0, index = 0;
  ANod nod = *trie;
  char cuvantCurent[50] = {0};
  char *aux = calloc(1, sizeof(char));
  for (i = 0; i < len; i++)
  { 
    index = get_index(word[i]);
    nod = nod->Copii[index];

    if (nod == NULL)
    { 
      gasit = 1;
      break;
    }
    else
    {
      aux[0] = nod->caracterCurent;
      strncat(cuvantCurent, aux, 1);
    }
  }

  if (gasit == 0)
  {
    fprintf(file, "%s\n", cuvantCurent);
  }
  else
  {
      if (strlen(cuvantCurent) == 0)
    {
      fprintf(file, "None\n");
    }
    else
    {
      fprintf(file, "%s\n", cuvantCurent);
    }
  }  
}

void find_all_with_prefix(FILE *file, ATrie trie, char pref[])
{
  int i, len = strlen(pref), gasit = 0, index = 0;
  char prefix[50] = {0};

  ANod nod = *trie;
  for (i = 0; i < len; i++)
  { 
    index = get_index(pref[i]);
    nod = nod->Copii[index];
   
    if (nod == NULL)
    {
      gasit = 1;
      break;
    }
  }  
  int j;

  if (len == 0)
  {
    printf("ec\n");
    char *aux = calloc(1, sizeof(char));
    for (j = 0; j < MAXCOPII; j++)
    {
      if (nod->Copii[j] != NULL)
      {
        prefix[0] = '\0';
        print_trie(file, nod->Copii[j], pref, aux, prefix);
      }
    }
    fprintf(file, "\n");
  }
  else
  {
    if (i == len)
    {
      fprintf(file, "%s ", pref);
    }
  }

  if (gasit == 1)
  {
    fprintf(file, "None\n");
  }
  else
  {
   if (len != 0)
   {
    //printf("ok\n");
    char *aux = calloc(1, sizeof(char));
    for (j = 0; j < MAXCOPII; j++)
    {
      //printf("ceva\n");
      if (nod->Copii[j] != NULL)
      {
        //printf("asa\n");
        //printf("%d\n", j);
        prefix[nod->nivel + 1] = '\0';
        print_trie(file, nod->Copii[j], pref, aux, prefix);
      }
    }
    fprintf(file, "\n");
   }
  }
}

void mean_length(FILE *file, ATrie trie, char pref[])
{
  int i, len = strlen(pref), gasit = 0, suma = 0, nrCuv = 0, index = 0;
  float rez = 0;

  ANod nod = *trie;
  for (i = 0; i < len; i++)
  { 
    index = get_index(pref[i]);
    nod = nod->Copii[index];
   
    if (nod == NULL)
    {
      gasit = 1;
      break;
    }
  }  
  int j;
  if (len == 0)
  {
    for (j = 0; j < MAXCOPII; j++)
    {
      if (nod->Copii[j] != NULL)
      {
        suma_trie(file, nod->Copii[j], &suma, &nrCuv);
        //printf("1 | %d %d \n", suma, nrCuv);
      }
    }
    rez = (float)suma/nrCuv;
    fprintf(file, "%.3f\n", rez);
  }
  else
  {
    if (gasit == 1)
    {
      fprintf(file, "0\n");
    }
    else
    {
      for (j = 0; j < MAXCOPII; j++)
      {
        if (nod->Copii[j] != NULL)
        {
          suma_trie(file, nod->Copii[j], &suma, &nrCuv);
          //printf("2 | %d %d \n", suma, nrCuv);
        }
      }
      rez = (float)suma/nrCuv;
      fprintf(file, "%.3f\n", rez);
    }
  }  
}