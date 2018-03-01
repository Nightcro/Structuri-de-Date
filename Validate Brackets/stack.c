#include "stack.h"

//Program realizat de Mindru Alexandru-Ionut
//Grupa 311CB

TStack *initS(size_t len, print prt)
{
	TStack *temp = malloc(sizeof(TStack));

	if (temp == NULL) 
	{	
		return NULL;
	}

	temp->bucket = AlocBucketS(len);

	if (temp->bucket == NULL)
	{
		return NULL;
	}

	temp->len = len; // se asociaza numarul maxim de stive
	temp->prt = prt; // se asociaza functia de printare pentru informatii

	return temp;
}

Stack *AlocBucketS(size_t len)
{
	Stack *temp = malloc(len  *sizeof(Stack));

	if (temp == NULL)
	{
		return NULL;
	}

	unsigned int i;

	for (i = 0; i < len; i++)
	{
		temp[i].lista = NULL;
	}

	return temp;
}

int Rastoarna(Stack *temp, Stack *backward)
{
	unsigned int error;

	while (temp->lista != NULL)
	{
		void *aux;
		// ignoram eroarea intoarsa deoarece pop intoarce 1
		// in momentul cand temp este NULL
		// ceea ce se verifica la inceput in while
		pop(temp, &aux); 
		error = push(backward, aux); 

		if (error == 1) // se verifica daca s-a facut push-ul corect
		{
			DistLS(&backward->lista);
			return 1;
		}
	}

	return 0;
}

int push(Stack *temp, void *info)
{
	TLG aux = AlocCelula(info);

	if (aux == NULL)
	{
		return 1;
	}

	if (temp->lista == NULL)
	{
		temp->lista = aux;
	}
	else
	{
		aux->urm = temp->lista;
		temp->lista = aux;
	}

	return 0;
}

int top(Stack *temp, void **aux)
{
	if (temp->lista == NULL)
	{
		return 1;
	}
	// se intoarce adresa informatiei fara a o elimina
	*aux = temp->lista->info;

    return 0;
}

int pop(Stack *temp, void **info)
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

int prints(TStack *temp, FILE *out)
{
	unsigned int error, i;

	for (i = 0; i < temp->len; i++)
	{
		fprintf(out, "\"");

		if (temp->bucket[i].lista != NULL)
		{
			Stack backward;
			backward.lista = NULL;
			error = Rastoarna(&temp->bucket[i], &backward);

			if (error)
			{
				return 1;
			}

			while (backward.lista != NULL)
			{
				void *aux;
				// la fel ca la rastoarna, se ignora eroarea de la pop
				pop(&backward, &aux);
				temp->prt(aux, out);
				error = push(&temp->bucket[i], aux);

				if (error)
				{
					// distrug backward, ce a reusit sa intre in lista poate fi
					DistLS(&backward.lista); // distrus la final de main
					return 1;
				}
			}
		}

		fprintf(out, "\"\n");
	}

	return 0;
}

int sorts_recur(Stack *temp, void *aux, cmp cmp_id, unsigned int error)
{ // oprirea unei recursivitati mai greu :(
	if (error == 1)
	{
		return 1;
	}

	void *info;
	unsigned int sem = 0, error_sem = top(temp, &info);

	if (error_sem == 0) // folosit pentru a determina daca elementul primit ca
	{ // parametru (aux) este mai mare ca ceea ce se afla deja in stiva 
		sem = cmp_id(aux, info); // intoarce 1 cmp_id daca e adevarat
	}

	if ((temp->lista == NULL) || (sem == 1)) // daca stiva e goala adaugam 
	{										// chiar daca sem e 0
		error = push(temp, aux);

		if (error == 1)
		{
			return 1;
		}

		return 0;
	}

	error = pop(temp, &info); // daca a trecut de conditia de mai sus inseamna
	// ca ceea ce se afla in stiva e mai mic si scoatem top-ul din el
	if (error == 1)
	{
		return 1;
	}
	// apelam sorts_recur din nou
	error = sorts_recur(temp, aux, cmp_id, error);

	if (error == 1)
	{
		return 1;
	}
	// ajungem in faza finala cand punem la loc top-ul
	error = push(temp, info);

	if (error == 1)
	{
		return 1;
	}

	return 0;
}
// trimit ca parametru eroarea pentru a verifica la inceput 
int sorts(Stack *temp, cmp cmp_id, unsigned int error)
{
	if (error == 1)
	{
		return 1;
	}

	if (temp->lista != NULL)
	{
		void *aux;
		error = pop(temp, &aux);

		if (error == 1)
		{
			return 1;
		}
		// prima etapa, se extrag toate adresele informatiei din stiva
		error = sorts(temp, cmp_id, error);

		if (error == 1)
		{
			free(aux);
			return 1;
		}
		// a doua etapa se apeleaza sorts_recur pentru a determina aranjarea
		error = sorts_recur(temp, aux, cmp_id, 0); // informatiei

		if (error == 1)
		{
			return 1;
		}
	}

	return 0;
}

unsigned int corrects(Stack *temp, cmp cmp_type, int *ans)
{ // multe... multe... conditii si erori...
	void *aux, *help, *index;
	int poz = 0;

	char type1_1 = '(', type1_2 = ')';
	char type2_1 = '[', type2_2 = ']';
	char type3_1 = '{', type3_2 = '}';

	Stack backward, aux_stack, int_stack;
	int_stack.lista = NULL; // stiva pentru indicii subsirilor
	backward.lista = NULL; // stiva originala e intoarsa si pusa in backward
	aux_stack.lista = NULL; // stiva folosit pentru a verifica inchidere corecta
							// de paranteza
	*ans = -1;
	void *result = malloc(sizeof(int));
	*(int*)result = *ans;
	*ans = 0;
	unsigned int error = push(&int_stack, result);
	// punem pe stiva de pozitia ^^ baza ca fiind -1
	if (error == 1)
	{
		free(result);
		return 1;
	}
	// e intoarsa stiva originala
	error = Rastoarna(temp, &backward);

	if (error == 1)
	{
		return 1;
	}

	while (backward.lista != NULL)
	{	// scoatem din stiva
		pop(&backward, &aux);
		// verificam daca se deschide o paranteza
		if ((cmp_type(aux, &type1_1)) || (cmp_type(aux, &type2_1)) 
			|| (cmp_type(aux, &type3_1)))
		{	// o punem pe stiva pentru a verifica daca se inchide corect
			error = push(&aux_stack, aux);

			if (error == 1)
			{
				while (int_stack.lista != NULL)
				{
					pop(&int_stack, &help);
					free(help);
				}

				DistLS(&aux_stack.lista);
				DistLS(&backward.lista);
				return 1;
			}
			// punem pozitia unde s-a deschis pe stiva de indici
			result = malloc(sizeof(int));
			*(int*)result = poz;
			error = push(&int_stack, result);

			if (error == 1)
			{
				while (int_stack.lista != NULL)
				{
					pop(&int_stack, &help);
					free(help);
				}

				free(result);
				DistLS(&aux_stack.lista);
				DistLS(&backward.lista);
				return 1;
			}
		}
		else
		{	
			unsigned int error_sem = top(&aux_stack, &help);
			// verificam daca paranteza se inchide corect in functie de tipul ei
			if ((error_sem == 0) && 
				(((cmp_type(help, &type1_1)) && (cmp_type(aux, &type1_2)))
			|| ((cmp_type(help, &type2_1)) && (cmp_type(aux, &type2_2)))
			|| ((cmp_type(help, &type3_1)) && (cmp_type(aux, &type3_2)))))
			{	// daca se inchide corect atunci dam afara opening-ul
				pop(&aux_stack, &help);
				pop(&int_stack, &result); // dam si pozitia initiala a
				free(result);			// opening-ului afara

				if (int_stack.lista != NULL)
				{	// verificam lungimea subsirului daca e cea mai mare
					top(&int_stack, &index);

					if (*ans < poz - *(int*)index)
					{ // se calculeaza lungimea subsirului gasit
						*ans = poz - *(int*)index;
					}
				}
			}
			else
			{	// daca nu se inchide corect dam totul afara din stiva de indici
				while (int_stack.lista != NULL)
				{
					pop(&int_stack, &help);
					free(help);
				}
				// punem pozitia curenta in stiva de indici de unde s-a stricat
				result = malloc(sizeof(int));
				*(int*)result = poz;
				error = push(&int_stack, result);

				if (error == 1)
				{
					while (int_stack.lista != NULL)
					{
						pop(&int_stack, &help);
						free(help);
					}

					free(result);
					DistLS(&aux_stack.lista);
					DistLS(&backward.lista);
					return 1;
				}
				// eliberez stiva auxiliara pentru inchidere corecta
				while (aux_stack.lista != NULL)
				{
					pop(&aux_stack, &help);
				}
			}
		}

		poz++; // pun in stiva originala in acelasi timp
		error = push(temp, aux);

		if (error == 1)
		{
			while (int_stack.lista != NULL)
			{
				pop(&int_stack, &help);
				free(help);
			}

			DistLS(&aux_stack.lista);
			DistLS(&backward.lista);
			return 1;
		}
	}

	while (int_stack.lista != NULL)
	{	// eliberez cea mai ramas in stiva de indici
		pop(&int_stack, &help);
		free(help);
	}

	while (aux_stack.lista != NULL)
	{	// eliberez si stiva de inchidere corecta a parantezelor
		pop(&aux_stack, &help);
	}

	return 0;
}

void DistS(TStack **temp)
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