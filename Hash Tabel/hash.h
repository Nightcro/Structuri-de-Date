#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "materie.h"
#include "student.h"
#include "lista.h"

//Program realizat de Mindru Alexandru-Ionut
//Grupa 311CB

typedef unsigned int (*TFHash)(const void*, size_t, size_t);
typedef int (*cmpKey)(void*, void*);
typedef void (*print)(void*, FILE*);
typedef void (*dist)(void*);
typedef size_t (*sizeK)(void*);
typedef void* (*readK)(FILE* in);

typedef struct
{
	void* val;	// valoarea de tip TStudent/TMaterie
	void* key;	// cheia asociata valorii
	print prt;	// functie de afisare a valorii
	dist fr;	// functie de eliberare a valorii
} Info;

typedef struct
{
	size_t len; // numarul actual de intrari in bucket
	size_t max_len; // numarul maxim de intrari alocat pentru bucket
	TFHash keyGenerator; // functia hash_f
	TLG* bucket; // vectorul de liste
	cmpKey cmp; // functie comparare
	print prtkey, prtTH; // functii de printare cheie, resp tabela
	dist freeKey; // functie de eliberare a memoriei cheii
	sizeK sizeKey; // functie pentru dimensiunea cheii
	readK readKey; // functie pentru citirea cheii
} Hash;

unsigned int hash_f(const void *data, size_t len, size_t range);
Hash* initHT(size_t max_len, char type_key);
TLG* AlocBucket(size_t len);
Info* AlocInfo(void* elem, void* key, size_t dim, size_t dimKey, char* type);
void DistTH(Hash** temp);
void insertVal(Hash* temp, FILE* in);
void printTH(void* temp, FILE* out);
void DistKey(Hash* temp, FILE* in);
void resizeTH(Hash* temp);
void findKey(Hash* temp, FILE* in, FILE* out);
void printKeyint(void* temp, FILE* out);
void printKeystring(void* temp, FILE* out);
int cmpKeystring(void* key1, void* key2);
int cmpKeyint(void* key1, void* key2);
size_t sizeKeystring(void* key);
size_t sizeKeyint(void* key);
void* readKeystring(FILE* in);
void* readKeyint(FILE* in);