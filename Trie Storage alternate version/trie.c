#include "trie.h"

unsigned char get_index(char* cuv)
{
	if (((*cuv) - '0' >= 0) && ((*cuv) - '0' <= 9))
	{
		return '0' - 26;
	}
	else
	{
		return 'a';
	}
}

Trie* InitTrie()
{
	Trie *temp = malloc(sizeof(Trie));

	if (temp == NULL)
	{
		return NULL;
	}

	temp->sons = malloc(MAX * sizeof(Trie*));

	if (temp->sons == NULL)
	{
		free(temp);
		return NULL;
	}

	temp->word = 0;
	temp->nrwords = 0;
	unsigned int i;

	for (i = 0; i < MAX; i++)
	{
		temp->sons[i] = NULL;
	}

	return temp;
}

unsigned int add(Trie *temp, char *cuv)
{
	if ((*cuv) == '\0')
	{
		temp->word = 1;
		return 0;
	}

	temp->nrwords++;
	unsigned char index = get_index(cuv);

	if (temp->sons[(*cuv) - index] == NULL)
	{
		temp->sons[(*cuv) - index] = InitTrie();
		
		if (temp->sons[(*cuv) - index] == NULL)
		{
			return 1;
		}

		return add(temp->sons[(*cuv) - index], cuv + 1);
	}
	else
	{
		return add(temp->sons[(*cuv) - index], cuv + 1);
	}
}

void remove_word_aux(Trie **temp, char *cuv)
{
	if ((*cuv) == '\0')
	{
		if ((*temp)->nrwords == 0)
		{
			free((*temp)->sons);
			free(*temp);
			*temp = NULL;
		}
		else
		{
			(*temp)->word = 0;
		}

		return;
	}

	remove_word_aux(&(*temp)->sons[(*cuv) - get_index(cuv)], cuv + 1);
	(*temp)->nrwords--;
	return;
}

void remove_word(Trie *temp, char *cuv)
{
	unsigned int error = find(temp, cuv);

	if (error == 1)
	{
		return;
	}

	remove_word_aux(&temp, cuv);

	return;
}

unsigned int find(Trie *temp, char *cuv)
{
	if ((*cuv) == '\0')
	{
		if (temp->word == 1)
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}

	unsigned char index = get_index(cuv);

	if (temp->sons[(*cuv) - index] == NULL)
	{
		return 1;
	}
	else
	{
		return find(temp->sons[(*cuv) - index], cuv + 1);
	}
}

char* find_longest_prefix(Trie *temp, char *cuv)
{
	unsigned char index = get_index(cuv);
	char *ans = NULL;

	if (temp->sons[(*cuv) - index] == NULL)
	{
		return ans;
	}

	unsigned int size = initial_size, pass;
	ans = malloc(size * sizeof(char));

	for (pass = 0, index = get_index(cuv); (*cuv) != '\0'; cuv++, pass++)
	{

		if ((((*cuv) - index) < 0) || (temp->sons[(*cuv) - index] == NULL))
		{
			break;
		}

		ans[pass] = *cuv;
		temp = temp->sons[(*cuv) - index];

		if (size == (pass + 1))
		{
			size *= 2;
			ans = realloc(ans, size * sizeof(char));
		}
	}

	ans[pass] = '\0';
	return ans;
}

int pref_aux(Trie **temp, char *cuv, char **pref)
{
	unsigned char index;
	unsigned int pass = 0, size = initial_size;
	*pref = malloc(size * sizeof(char));

	if (cuv != NULL)
	{
		for (pass = 0, index = get_index(cuv); (*cuv) != '\0'; cuv++, pass++)
		{
			if ((((*cuv) - index) < 0) || 
				((*temp)->sons[(*cuv) - index] == NULL))
			{
				free(*pref);
				return 1;
			}

			(*pref)[pass] = *cuv;
			*temp = (*temp)->sons[(*cuv) - index];

			if (size == (pass + 1))
			{
				size *= 2;
				*pref = realloc(*pref, size * sizeof(char));
			}
		}
	}

	(*pref)[pass] = '\0';
	return 0;
}

void find_all_with_prefix_aux(Trie *temp, FILE *out, char **pref, 
								unsigned int size)
{
	if (temp->word == 1)
	{
		fprintf(out, "%s ", *pref);
	}

	unsigned int i, pass;
	char word;

	for (i = 0; i < MAX; i++)
	{
		if (temp->sons[i] != NULL)
		{
			if (i <= 25)
			{
				word = i + 'a';
			}
			else
			{
				word = i + '0' - 26;
			}

			pass = strlen(*pref);

			if (size == (pass + 1))
			{
				size *= 2;
				*pref = realloc(*pref, size * sizeof(char));
			}

			(*pref)[pass] = word;
			(*pref)[pass + 1] = '\0';
			find_all_with_prefix_aux(temp->sons[i], out, pref, size);
			(*pref)[pass] = '\0';
		}
	}

	return;
}

void find_all_with_prefix(Trie *temp, char *cuv, FILE *out)
{
	char *pref;
	unsigned int error;
	error = pref_aux(&temp, cuv, &pref);

	if (error == 1)
	{
		fprintf(out, "None\n");
		return;
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

	for (i = 0; i < MAX; i++)
	{
		if (temp->sons[i] != NULL)
		{
			size++;
			mean_length_aux(temp->sons[i], size, sum, nr);
			size--;
		}
	}

	return;
}

float mean_length(Trie *temp, char *cuv)
{
	char *pref;
	unsigned int error;
	error = pref_aux(&temp, cuv, &pref);

	if (error == 1)
	{
		return 0;
	}

	unsigned int sum = 0, nr = 0;
	mean_length_aux(temp, strlen(pref), &sum, &nr);
	free(pref);

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

	for (i = 0; i < MAX; i++)
	{
		DistTrie(&(*temp)->sons[i]);
	}

	free((*temp)->sons);
	free(*temp);
	return;
}