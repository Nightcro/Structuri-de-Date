 #include "hash.h"

//Program realizat de Mindru Alexandru-Ionut
//Grupa 311CB

/*
 * data - key to compute hash
 * len - sizeof of the data
 * range - the number of buckets
 * Return the index of the bucket
 */
unsigned int hash_f(const void *data, size_t len, size_t range) 
{
	unsigned int hash = 0u;
	size_t idx = 0;
	size_t cof = 1;
	size_t rate = 2;
	const char *d = (const char*)data;
	for( ; idx < len; ++idx) 
	{
		hash += d[idx] * cof;
		cof *= rate;
	}
	return hash % range;
}

Hash* initHT(size_t max_len, char type_key)
{
	Hash* temp = malloc(sizeof(Hash));

	if (!temp) 
	{	
		printf("Eroare alocare HashTable.\n");
		return NULL;
	}

	temp->bucket = AlocBucket(max_len);
	temp->len = 0;
	temp->max_len = max_len;
	temp->keyGenerator = hash_f;
	temp->prtTH = printTH;
	temp->freeKey = free;

	if (type_key == 'd')
	{
		temp->cmp = cmpKeyint;
		temp->prtkey = printKeyint;
		temp->sizeKey = sizeKeyint;
		temp->readKey = readKeyint;
	}
	else
	{
		temp->cmp = cmpKeystring;
		temp->prtkey = printKeystring;
		temp->sizeKey = sizeKeystring;
		temp->readKey = readKeystring;
	}

	return temp;
}

TLG* AlocBucket(size_t len)
{
	TLG* temp = malloc(len * sizeof(TLG));
	unsigned int i;

	if (!temp)
	{
		printf("Eroare alocare vector de pointeri TLG in HashTable.\n");
		return NULL;
	}

	for (i = 0; i < len; i++)
	{
		temp[i] = NULL;
	}

	return temp;
}

Info* AlocInfo(void* elem, void* key, size_t dim, size_t dimKey, char* type)
{
	Info* temp = malloc(sizeof(Info));

	if (!temp)
	{
		return NULL;
	}
	if (!strcmp(type, "stud"))
	{
		temp->prt = printStud;
		temp->fr = freeStud;
	}
	else
	{
		temp->prt = printMat;
		temp->fr = freeMat;
	}

	temp->key = malloc(dimKey);

	if (!temp->key)
	{ 
		free(temp); 
		return NULL;
	}

	memcpy(temp->key, key, dimKey);
	free(key);
	temp->val = malloc(dim);

	if (!temp->val)
	{ 
		free(temp->key);
		free(temp); 
		return NULL;
	}

	memcpy(temp->val, elem, dim);
	free(elem);
	return temp;
}

void insertVal(Hash* temp, FILE* in)
{
	float factor;
	char type[5];
	void* key = temp->readKey(in);
	size_t dim_key = temp->sizeKey(key);
	fscanf(in, "%s", type);
	unsigned int code = temp->keyGenerator(key, dim_key, temp->max_len);
	TLG aux;
	Info* old = NULL;

	for (aux = temp->bucket[code]; aux != NULL; aux = aux->urm)
	{
		if (!temp->cmp(((Info*)aux->info)->key, key))
		{ // daca se gaseste o cheie 
			old = (Info*)aux->info; 
		} // asemanatoare in bucket se memoreaza adresa
	}
	if (!strcmp(type, "stud"))
	{
		TStudent* val = readStud(in);
		if (old != NULL)
		{
			old->fr(old->val);
			old->prt = printStud; // se elibereaza si se inlocuieste
			old->fr = freeStud;	// informatia de la adresa gasita
			memcpy(old->val, val, sizeof(TStudent));
			free(val);
			free(key);
		}
		else
		{
			Info* inf = AlocInfo(val, key, sizeof(TStudent), dim_key, type);
			int error = InsertTLG(&(temp->bucket[code]), inf, sizeof(Info));
			temp->len++;
			if (error)
			{
				printf("Eroare inserare!\n");
				return;
			}
		}
	}
	else
	{
		TMaterie* val = readMat(in);
		if (old != NULL)
		{
			old->fr(old->val);
			old->prt = printMat; // se elibereaza si se inlocuieste
			old->fr = freeMat;	// informatia de la adresa gasita
			memcpy(old->val, val, sizeof(TMaterie));
			free(val);
			free(key);
		}
		else
		{
			Info* inf = AlocInfo(val, key, sizeof(TMaterie), dim_key, type);
			int error = InsertTLG(&(temp->bucket[code]), inf, sizeof(Info));
			temp->len++;
			if (error)
			{
				printf("Eroare inserare!\n");
				return;
			}
		}
	}

	factor = (float)temp->len / temp->max_len;

	if (factor >= 0.75)
	{
		resizeTH(temp);
	}

	return;
}

void DistTH(Hash** temp)
{
	unsigned int i;

	for (i = 0; i < (*temp)->max_len; i++)
	{
		DistLS(&((*temp)->bucket[i]), (*temp)->freeKey);
	}

	free((*temp)->bucket);
	free(*temp);
	return;
}

void DistKey(Hash* temp, FILE* in)
{
	void* key = temp->readKey(in);
	size_t dim_key = temp->sizeKey(key);
	unsigned int code = temp->keyGenerator(key, dim_key, temp->max_len);
	TLG aux = temp->bucket[code];

	if (aux != NULL)
	{
		Info* check = (Info*)(aux->info);
		if (!temp->cmp(check->key, key))
		{
			temp->bucket[code] = aux->urm;
			temp->freeKey(((Info*)(aux->info))->key);
			DistCelula(&aux);	// se verifica daca ceea ce caut se
			temp->len--;		// afla pe prima celula din lista
		}
		else
		{
			for ( ; aux != NULL; aux = aux->urm)
			{
				if (aux->urm != NULL)
				{	
					check = (Info*)(aux->urm->info);
					if (!temp->cmp(check->key, key))
					{
						TLG back = aux->urm;
						temp->freeKey(((Info*)(back->info))->key);
						aux->urm = back->urm; // se verifica in avans daca
						DistCelula(&back); // cheia pe care o caut se afla
						temp->len--; // in celula urmatoare pentru a o elimina
					}				 // si a reface legaturile
				}
			}
		}
	}

	free(key);
	return;
}

void resizeTH(Hash* temp)
{
	unsigned int old_len = temp->max_len, i;
	temp->max_len = temp->max_len * 2;
	TLG* new_bucket = AlocBucket(temp->max_len);
	TLG* old_bucket = temp->bucket;

	for (i = 0; i < old_len; i++)
	{
		if (old_bucket[i] != NULL)
		{
			TLG cell = old_bucket[i];
			for ( ; cell != NULL; cell = cell->urm)
			{
				void* key = (*(Info*)(cell->info)).key;
				size_t dim_key = temp->sizeKey(key); // copiem ce se afla
				unsigned int code = temp->keyGenerator(key, dim_key, 
									temp->max_len); // in bucketul vechi
				int error = InsertTLG(&(new_bucket[code]), cell->info, 
										sizeof(Info)); // in cel nou
				if (error)
				{
					printf("Eroare inserare redimensionare!\n");
					return;
				}
			}
		}
	}

	temp->bucket = new_bucket;

	for (i = 0; i < old_len; i++) // eliberam vechiul bucket
	{
		while (old_bucket[i] != NULL)
		{
			TLG back = old_bucket[i];
			old_bucket[i] = old_bucket[i]->urm;
			free(back);
		}
	}

	free(old_bucket);
	return;
}

void findKey(Hash* temp, FILE* in, FILE* out)
{
	void* key = temp->readKey(in);
	size_t dim_key = temp->sizeKey(key);
	unsigned int code = temp->keyGenerator(key, dim_key, temp->max_len);
	TLG aux; 
	Info* cell = NULL;

	for (aux = temp->bucket[code]; aux != NULL; aux = aux->urm)
	{
		if (!temp->cmp(((Info*)aux->info)->key, key))
		{
			cell = (Info*)aux->info;
			cell->prt(cell->val, out);
			fprintf(out, "\n");
			free(key);
			return;
		}
	}

	fprintf(out, "Nu exista\n");
	free(key);
	return;
}

void printTH(void* table, FILE* out)
{
	Hash temp = *((Hash*)table);
	TLG aux;
	unsigned int i;

	for (i = 0; i < temp.max_len; i++)
	{
		fprintf(out, "[%d] :", i);	
		for (aux = temp.bucket[i]; aux != NULL; aux = aux->urm)
		{
			fprintf(out, " (");
			temp.prtkey(aux->info, out);
			fprintf(out, " -> ");
			Info* inf = (Info*)(aux->info);
			inf->prt(inf->val, out);
			fprintf(out, ")");
		}
		fprintf(out, "\n");
	}

	return;
}

void printKeystring(void* temp, FILE* out)
{
	char* aux = (char*)(((Info*)temp)->key);
	fprintf(out, "%s", aux);
	return;
}

void printKeyint(void* temp, FILE* out)
{
	unsigned int* aux = (unsigned int*)(((Info*)temp)->key);
	fprintf(out, "%d", *aux);
	return;
}

int cmpKeystring(void* key1, void* key2)
{
	return strcmp((char*)key1, (char*)key2);
}

int cmpKeyint(void* key1, void* key2)
{
	if ((*(unsigned int*)key1) == (*(unsigned int*)key2))
	{
		return 0;	// 0 reprezinta valoarea de adevar, la fel ca la strcmp
	}				// am luat asa ca sa nu am probleme la comparare 
	return 1;		// indiferent daca ar fi string sau int
}

size_t sizeKeystring(void* key)
{
	return strlen((char*)key) + 1;
}

size_t sizeKeyint(void* key)
{
	return sizeof(*((unsigned int*)key));
}

void* readKeystring(FILE* in)
{

	char key[50];
	fscanf(in, "%s", key);
	char* new_key = malloc((strlen(key) + 1) * sizeof(char));
	// citesc intr-o valoare statica pentru a aloca dinamic cat mai eficient
	if (new_key == NULL)
	{
		return NULL;
	}

	memcpy(new_key, key, (strlen(key) + 1));
	return new_key;
}

void* readKeyint(FILE* in)
{
	unsigned int* key = malloc(sizeof(unsigned int));
	fscanf(in, "%u", key);
	return key;
}