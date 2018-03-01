#include "trie.h"

//Program realizat de Mindru Alexandru-Ionut
//Grupa 311CB

Trie* InitTrie()
{	// functie de intializare pentru structura de Trie
	Trie *temp = malloc(sizeof(Trie));

	if (temp == NULL)
	{
		return NULL;
	}

	temp->sons = NULL;
	temp->word = 0;
	temp->nrwords = 0;
	temp->nrsons = 0;

	return temp;
}

unsigned int add(Trie *temp, char *cuv)
{
	unsigned int i;

	for (i = 0; i < temp->nrsons; i++)
	{
		if (temp->sons[i]->letter == *cuv)
		{	// se plimba prin litere care exista deja in arbore
			if (*(cuv + 1) == '\0')
			{	// daca cuvantul pe care incerc sa-l adaug are literele
				temp->sons[i]->word = 1; // existente in trie atunci
				return 0; // pe nodul respectiv in care se afla ultima litera
			}	// o desemnez ca sfarsit de cuvant (o marchez cu 1)

			return add(temp->sons[i], cuv + 1);
		}
	}
	// daca a trecut de for inseamna ca trebuie sa adaug litere noi pe calea
	temp->nrwords++; // respectiva
	temp->nrsons++;
	temp->sons = realloc(temp->sons, temp->nrsons * sizeof(Trie*));
	// ^^ realloc vectorul cu un fiu in plus
	if (temp->sons == NULL)
	{
		return 1;
	}
	// adaug in ordina alfabetica, shiftez la dreapta pana gasesc pozitia buna
	for (i = 0; i < temp->nrsons - 1; i++)
	{
		if (temp->sons[i]->letter > *cuv)
		{
			unsigned int j;

			for (j = temp->nrsons - 1; j > i; j--)
			{
				temp->sons[j] = temp->sons[j - 1];
			}

			break;
		}
	}

	temp->sons[i] = InitTrie();

	if (temp->sons[i] == NULL)
	{
		return 1;
	}

	temp->sons[i]->letter = *cuv;

	if (*(cuv + 1) == '\0')
	{	// verificam in avans daca s-a terminat cuvantul
		temp->sons[i]->word = 1;
		return 0;
	}
	// reapelam functia pentru literele ramase
	return add(temp->sons[i], cuv + 1);
}

void remove_word_aux(Trie **temp, char *cuv)
{
	if ((*cuv) == '\0')
	{
		if ((*temp)->nrwords == 0)
		{	// daca nr de cuvinte pe calea respectiva e 0 inseamna ca pot sa
			free(*temp); // elimin nodurile
			*temp = NULL;
		}
		else
		{	// daca e diferit de 0 inseamna ca ma afar in interiorul grafului
			(*temp)->word = 0; // si mai sunt alte cuvinte pe calea aceasta
		}	// doar fac nodul sa nu mai reprezinte sfarsitul unui cuvant

		return;
	}

	unsigned int i;

	for (i = 0; i < (*temp)->nrsons; i++)
	{
		if((*temp)->sons[i]->letter == *cuv)
		{
			remove_word_aux(&(*temp)->sons[i], cuv + 1);

			if ((*temp)->sons[i] == NULL)
			{	// daca am eliminat nodul respectiv shiftez la stanga
				(*temp)->nrsons--;
				unsigned int j;

				for (j = i; j < (*temp)->nrsons; j++)
				{
					(*temp)->sons[j] = (*temp)->sons[j + 1];
				}

				i -= 1;
			}

			(*temp)->nrwords--;
		}
	}

	if (((*temp)->nrsons == 0) && ((*temp)->nrwords == 0))
	{	// elimin nodul in cazul in care nu mai exista fii si cuvinte pe cale
		free((*temp)->sons);
		free(*temp);
		*temp = NULL;
	}

	return;
}

void remove_word(Trie *temp, char *cuv)
{	// folosesc find mai intai sa vad daca exista cuvantul in trie
	unsigned int error = find(temp, cuv);

	if (error == 1)
	{
		return;
	}

	remove_word_aux(&temp, cuv);

	return;
}

unsigned int find(Trie *temp, char *cuv)
{	// verific mai intai daca am ajuns la finalul cuvantului
	if ((*cuv) == '\0')
	{
		if (temp->word == 1)
		{	// nod in care se termina cuvantul
			return 0;
		}
		else
		{	// am ajuns la final dar nu se termina cuvantul in acest nod
			return 1; // => nu exista cuvantul in trie
		}
	}

	unsigned int i;

	for (i = 0; i < temp->nrsons; i++)
	{	// caut cuvantul in trie
		if (temp->sons[i]->letter == *cuv)
		{
			return find(temp->sons[i], cuv + 1);
		}
	}

	return 1;
}

char* find_pref(Trie **temp, char **cuv, unsigned int *pass)
{	// functie auxiliara a gasirii prefixului folosita de mai multe functii
	unsigned int i, size = initial_size;
	*pass = 0; // dimensiunea prefixului cand are litere
	char *pref = malloc(size * sizeof(char));

	if (*cuv != NULL)
	{
		for (i = 0; i < (*temp)->nrsons; )
		{
			if ((*temp)->sons[i]->letter == *(*cuv))
			{
				pref[(*pass)++] = *(*cuv)++;
				(*temp) = (*temp)->sons[i];
				i = 0;

				if (size == (*pass + 1))
				{
					size *= 2;
					pref = realloc(pref, size * sizeof(char));
				}
			}
			else
			{
				i++;
			}
		}
	}

	return pref;
}

char* find_longest_prefix(Trie *temp, char *cuv)
{
	unsigned int pass;
	Trie *aux = temp;
	char *ans = find_pref(&aux, &cuv, &pass);
	// returnam prefixul gasit chiar daca nu e in totalitate ca prefixul citit
	if (pass == 0) // din fisier
	{
		free(ans);
		return NULL;
	}

	ans[pass] = '\0';
	return ans;
}

char* pref_aux(Trie **temp, char *cuv)
{
	unsigned int pass;
	char *pref = find_pref(temp, &cuv, &pass);	
	// returnam prefixul gasit doar daca e in totalitate ca prefixul citit
	if ((*cuv != '\0') || (pass == 0)) // din fisier
	{
		free(pref);
		return NULL;
	}

	pref[pass] = '\0';
	return pref;
}

void find_all_with_prefix_aux(Trie *temp, FILE *out, char **pref, 
								unsigned int size)
{
	if (temp->word == 1)
	{	// daca am ajung pe un nod care se termina litera atunci afisez cuvantul
		fprintf(out, "%s ", *pref);
	}

	unsigned int i, pass;

	for (i = 0; i < temp->nrsons; i++)
	{
		pass = strlen(*pref);

		if (size == (pass + 1))
		{
			size *= 2;
			*pref = realloc(*pref, size * sizeof(char));
		}

		(*pref)[pass] = temp->sons[i]->letter; // atribui in avans literele 
		(*pref)[pass + 1] = '\0'; // pun null terminator cuvantului temporar
		find_all_with_prefix_aux(temp->sons[i], out, pref, size);
		(*pref)[pass] = '\0'; // scot ultima litera din string
	}

	return;
}

void find_all_with_prefix(Trie *temp, char *cuv, FILE *out)
{
	char *pref = NULL;

	if (cuv != NULL)
	{
		pref = pref_aux(&temp, cuv);

		if (pref == NULL)
		{	// daca prefixul intors de pref_aux e NULL inseamna ca nu exista
			fprintf(out, "None\n"); // un astfel de prefix in trie
			return;
		}
	}
	else
	{	// daca nu exista niciun prefix atunci afisez tot ce se afla in trie
		pref = malloc(initial_size * sizeof(char));
		pref[0] = '\0';
	}

	find_all_with_prefix_aux(temp, out, &pref, initial_size);
	free(pref);
	fprintf(out, "\n");
	return;
}

void mean_length_aux(Trie *temp, unsigned int size, unsigned int *sum, 
						unsigned int *nr)
{
	if (temp->word == 1)
	{
		(*nr)++;
		(*sum) += size;
	}

	unsigned int i;

	for (i = 0; i < temp->nrsons; i++)
	{	// acelasi principiu ca la find_all_with_prefix
		size++;
		mean_length_aux(temp->sons[i], size, sum, nr);
		size--;
	}

	return;
}

float mean_length(Trie *temp, char *cuv)
{
	char *pref = NULL, size;
	unsigned int sum = 0, nr = 0;

	if (cuv != NULL)
	{	// se face mean_lenght pe cuvintele cu prefixul citit din fisier
		pref = pref_aux(&temp, cuv);

		if (pref == NULL)
		{
			return 0;
		}

		size = strlen(pref); // ma folosesc de lungimea cuvantului
		free(pref);
		mean_length_aux(temp, size, &sum, &nr);
	}
	else
	{	// se face mean_lenght pe tot trie-ul
		mean_length_aux(temp, 0, &sum, &nr);
	}

	if (nr == 0)
	{
		return 0;
	}

	return (float)sum / nr;
}

void DistTrie(Trie **temp)
{
	if (*temp == NULL)
	{
		return;
	}

	unsigned int i;

	for (i = 0; i < (*temp)->nrsons; i++)
	{
		DistTrie(&(*temp)->sons[i]);
	}

	free((*temp)->sons);
	free(*temp);
	return;
}