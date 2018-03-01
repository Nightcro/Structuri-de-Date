#include "hash.h"
#include "lista.h"

//Program realizat de Mindru Alexandru-Ionut
//Grupa 311CB

int InsertTLG(ALG temp, void* cell, size_t dim)
{
	TLG aux = malloc(sizeof(TCelulaG));

	if (!aux)
	{
		return 1;
	}

	aux->info = malloc(dim);

	if (!aux->info)
	{ 
		free(aux); 
		return 1;
	}

	memcpy(aux->info, cell, dim);
	free(cell);

	TLG back = *temp;
	if (back != NULL)
	{
		for ( ; back->urm != NULL; back = back->urm);
		back->urm = aux;
	}
	else
	{
		*temp = aux;
	}

	aux->urm = NULL;
	return 0;
}

void DistCelula(ALG temp)
{
	Info* aux = (Info*)((*temp)->info);
	aux->fr(aux->val);
	free(aux->val);
	free(aux);
	free(*temp);
	return;
}

void DistLS(ALG temp, dist fr)
{
	while (*temp != NULL)
	{
		TLG back = *temp;
		fr(((Info*)(back->info))->key);
		*temp = (*temp)->urm;
		DistCelula(&back);
	}
	return;
}