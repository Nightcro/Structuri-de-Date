#include "common.h"

//Program realizat de Mindru Alexandru-Ionut
//Grupa 311CB

Info *AlocInfo(void *elem, size_t dim)
{
	Info *temp = malloc(sizeof(Info));

	if (temp == NULL)
	{
		return NULL;
	}

	temp->fr = free;// se atribuie functie de eliberare a valorii din informatie
	temp->val = malloc(dim);// se aloca memorie valorii

	if (temp->val == NULL)
	{ 
		free(temp); 
		return NULL;
	}

	memcpy(temp->val, elem, dim);// se copiaza valoarea

	return temp;
}

int ReadVal(void *temp, FILE *in, char *op)
{
	unsigned int id_ls, id;
	char type;
	fscanf(in, "%u %u %c", &id_ls, &id, &type);
	// se citeste informatia si se pune intr-o structura de tipul Bracket
	Bracket aux_br;
	aux_br.id = id;
	aux_br.type = type;

	void *aux_info = AlocInfo(&aux_br, sizeof(Bracket));
	// se pune informatia intr-o structura de tipul info cu propria functie
	if (aux_info == NULL) // de stergere
	{
		return 1;
	}
	
	unsigned int error = 0;

	if (strcmp(op, "push") == 0)
	{
		error = push(&((TStack*)temp)->bucket[id_ls], aux_info);
	}
	else // se apeleaza functia indicata de operatie
	{
		error = intrq(&((TQueue*)(temp))->bucket[id_ls], aux_info);
	}	

	if (error == 1)
	{
		return 2;
	}

	return 0;
}

void PrintVal(void *info, FILE *out)
{
	Bracket temp = *(Bracket*)(((Info*)info)->val);
	fprintf(out, "%c", temp.type);
	return;
}

int cmp_brackets_type(void *a, void *b)
{
	Bracket x = *(Bracket*)(((Info*)a)->val);
	// folosit pentru a compara tipurile de paranteza
	if (x.type == *(char*)b)
	{
		return 1;
	}

	return 0;
}

int cmp_brackets_id(void *a, void *b)
{
	Bracket x = *(Bracket*)(((Info*)a)->val);
	Bracket y = *(Bracket*)(((Info*)b)->val);
	// folosit pentru sorts, intoarcem 1 doar pentru cazul in care primul
	if (x.id > y.id)// element e mai mare ca al doilea, restul nu ne intereseaza
	{
		return 1;
	}

	return 0;
}