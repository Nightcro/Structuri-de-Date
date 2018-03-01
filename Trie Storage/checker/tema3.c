#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "trie_operations.c"

#define MAXCOPII 36

void choose_option (FILE *in, FILE *out, ATrie trie, char option[])
{
  char word[50] = {0}, pref[50] = {0};

  if (strcmp(option, "add") == 0)  
  { 
    fscanf(in, "%s", word);
    //printf("%s\n", word);
    add(trie, word);
  }
  
  if (strcmp(option, "remove") == 0)
  {
    fscanf(in, "%s", word);
    //printf("%s", word);
    delete(trie, word);
  }
  
  if (strcmp(option, "find") == 0)
  {
    fscanf(in, "%s", word);
    //printf("word: %s\n", word);
    find(out, trie, word);
  }
  
  if (strcmp(option, "find_longest_prefix") == 0)
  {
    fscanf(in, "%s", word);
    //printf("word: %s\n", word);
    find_longest_prefix(out, trie, word);
  }
  
  if (strcmp(option, "find_all_with_prefix") == 0)
  {
    int i, j = 0;
    fgets(pref, 50, in);
    int len = strlen(pref);
    char prefix[50] = {0};

    for (i = 0; i < len; i++)
    {
      if (pref[i] != ' ' && pref[i] != '\n')
      {
        prefix[j] = pref[i];
        j++; 
      }
    }

    //printf("pref: %s\n", prefix);
    find_all_with_prefix(out, trie, prefix);
  }
  
  if (strcmp(option, "mean_length") == 0)
  {
    int i, j = 0;
    fgets(pref, 50, in);
    int len = strlen(pref);
    char prefix[50] = {0};

    for (i = 0; i < len; i++)
    {
      if (pref[i] != ' ' && pref[i] != '\n')
      {
        prefix[j] = pref[i];
        printf("f = %d\n", j);
        if (j < 36)
        {
          j++;
        }
      }
    }

    //printf("pref: %s\n", prefix);
    mean_length(out, trie, prefix);
  }
}

int main(int argc, char *argv[])
{
    FILE *in = fopen(argv[1], "r");
    FILE *out = fopen(argv[2], "w");

    int nr_operatii;

    fscanf(in, "%d", &nr_operatii);
    
    int i;
    char option[30] = {0};

    ATrie trie = (ATrie)calloc(1, sizeof(ANod));
    *trie = (ANod)calloc(1, sizeof(TNod));
    (*trie)->Copii = calloc(MAXCOPII, sizeof(TNod));
    (*trie)->parinte = NULL;

    for (i = 0; i < MAXCOPII; i++)
    {
      (*trie)->Copii[i] = NULL;
    }

    (*trie)->nrCopii = 0;
    (*trie)->caracterCurent = ' ';
    (*trie)->FlagFinal = 0;
    (*trie)->nivel = -1;
    
    for (i = 0; i < nr_operatii; i++)
    {
      fscanf(in, "%s", option);
      choose_option(in, out, trie, option);
    }

    removeCopii(*trie);
    free((*trie)->Copii);
    free(*trie);
    free(trie);

    fclose(in);
    fclose(out);

    return 0;
}