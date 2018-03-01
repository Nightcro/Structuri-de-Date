#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 36
#define initial_size 10

typedef struct Trie
{
	int nrwords;
	unsigned char word;
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