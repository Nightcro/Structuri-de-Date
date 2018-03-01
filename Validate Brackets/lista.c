#include "common.h"
#include "lista.h"

//Program realizat de Mindru Alexandru-Ionut
//Grupa 311CB

TLG AlocCelula(void *info)
{
	TLG aux = malloc(sizeof(TCelulaG));

	if (aux == NULL)
	{
		return NULL;
	}

	aux->urm = NULL;
	aux->info = info;

	return aux;
}

void DistCelula(ALG temp)
{
	Info *aux = (Info*)((*temp)->info);
	aux->fr(aux->val);
	free(aux);
	free(*temp);
	return;
}

void DistLS(ALG temp)
{
	while (*temp != NULL)
	{
		TLG back = *temp;
		*temp = (*temp)->urm;
		DistCelula(&back);
	}

	return;
}