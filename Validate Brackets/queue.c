#include "queue.h"
#include "stack.h"

//Program realizat de Mindru Alexandru-Ionut
//Grupa 311CB

TQueue *initQ(size_t len, print prt)
{
	TQueue *temp = malloc(sizeof(TQueue));

	if (temp == NULL) 
	{	
		return NULL;
	}

	temp->bucket = AlocBucketQ(len);

	if (temp->bucket == NULL)
	{
		return NULL;
	}

	temp->len = len; // se asociaza numarul maxim de stive
	temp->prt = prt; // se asociaza functia de printare pentru informatii

	return temp;
}

Queue *AlocBucketQ(size_t len)
{
	Queue *temp = malloc(len  *sizeof(Queue));
	unsigned int i;

	if (temp == NULL)
	{
		return NULL;
	}

	for (i = 0; i < len; i++)
	{
		temp[i].lista = NULL;
		temp[i].ultim = NULL;
	}

	return temp;
}

int intrq(Queue *temp, void *info)
{
	TLG aux = AlocCelula(info);

	if (aux == NULL)
	{
		return 1;
	}

	if (temp->lista == NULL)
	{
		temp->lista = aux;
		temp->ultim = aux;
	}
	else
	{
		temp->ultim->urm = aux;
		temp->ultim = aux;
	}

	return 0;
}

int front(Queue *temp, void **aux)
{
	if (temp->lista == NULL)
	{
		return 1;
	}
	// se intoarce adresa informatiei fara a o elimina
	*aux = temp->lista->info;

    return 0;
}

int extrq(Queue *temp, void **info)
{
	if (temp->lista == NULL)
	{
		return 1;
	}

	TLG back = temp->lista;
	temp->lista = back->urm;
	*info = back->info;
	free(back);
	return 0;
}

int printq(TQueue *temp, FILE *out)
{
	unsigned int i, error;

	for (i = 0; i < temp->len; i++)
	{
		fprintf(out, "\"");

		if (temp->bucket[i].lista != NULL)
		{
			Queue backward;
			backward.lista = NULL;

			while (temp->bucket[i].lista != NULL)
			{
				void *aux; // in timp ce se afiseaza coada
				extrq(&temp->bucket[i], &aux); // salvam adresele informatiilor
				temp->prt(aux, out); // intr-o alta coada
				error = intrq(&backward, aux);

				if (error == 1)
				{
					DistLS(&backward.lista);
					return 1;
				}
			}

			temp->bucket[i] = backward; // atribuim coada in care am salvat
		}	// cozii originale

		fprintf(out, "\"\n");
	}

	return 0;
}

int sortq(Queue *temp, cmp cmp_id, unsigned int error)
{
	void *aux;
	Stack aux_stack, backward;
	aux_stack.lista = NULL;
	backward.lista = NULL;
	
	while (temp->lista != NULL) // punem coada intr-o stiva
	{
		extrq(temp, &aux);
		push(&aux_stack, aux); 
	}

	error = sorts(&aux_stack, cmp_id, 0); // sortam stiva

	if (error == 1)
	{
		return 1;
	}

	temp->ultim = aux_stack.lista; // salvam adresa ultimului element
	error = Rastoarna(&aux_stack, &backward); 
	// intoarcem stiva, ca sa o facem coada
	if (error == 1)
	{
		DistLS(&aux_stack.lista);
		return 1;
	}

	temp->lista = backward.lista; // salval adresa de inceput a cozii

	return 0;
}

unsigned int correctq(Queue *temp, cmp cmp_type, int *ans)
{
	unsigned int error;
	void *aux;
	Queue backup = *temp;
	backup.lista = NULL;
	Stack aux_stack;
	aux_stack.lista = NULL;
	
	while (temp->lista != NULL)
	{
		extrq(temp, &aux);
		error = push(&aux_stack, aux); // punem coada intr-o stiva

		if (error == 1)
		{
			DistLS(&aux_stack.lista);
			return 1;
		}

		error = intrq(&backup, aux); // si in acelasi timp si intr-o alta coada
									// eficient cand lucrezi cu adrese :)
		if (error == 1)
		{
			DistLS(&aux_stack.lista);
			DistLS(&backup.lista);
			return 1;
		}
	}

	error = corrects(&aux_stack, cmp_type, ans); // aplicam corrects pe stiva

	if (error == 1)
	{
		DistLS(&aux_stack.lista);
		return 1;
	}

	while (aux_stack.lista != NULL)
	{
		pop(&aux_stack, &aux); // facem pop la stiva, pentru a elimina celulele
	}							// fara a pierde adresa informatiei

	*temp = backup;

	return 0;
}

void DistQ(TQueue **temp)
{
	unsigned int i;

	for (i = 0; i < (*temp)->len; i++)
	{
		DistLS(&(*temp)->bucket[i].lista);
	}

	free((*temp)->bucket);
	free(*temp);
	return;
}