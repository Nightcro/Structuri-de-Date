#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Program realizat de Mindru Alexandru-Ionut
//Grupa 311CB

#define initial_size 10 // folosit pentru alocarea initiala a unor stringuri

typedef struct Trie
{
	unsigned int nrwords, nrsons; // variabile explicate mai bine in README
	unsigned char letter, word; // e mult de scris
	struct Trie **sons;
} Trie;

Trie* InitTrie();
unsigned int add(Trie *temp, char *cuv);
void remove_word(Trie *temp, char *cuv);
unsigned int find(Trie *temp, char *cuv);
char* find_longest_prefix(Trie *temp, char *cuv);
void find_all_with_prefix(Trie *temp, char *cuv, FILE *out);
float mean_length(Trie *temp, char *cuv);
void DistTrie(Trie **temp);