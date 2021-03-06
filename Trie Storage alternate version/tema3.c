#include "trie.h"

char* get_word(FILE *in)
{
	unsigned int size = initial_size, pass;
	char *cuv = malloc(size * sizeof(char));
	char aux;

	for (pass = 0; ((aux = getc(in)) != '\n') && (aux != EOF); pass++)
	{
		cuv[pass] = aux;

		if (size == (pass + 1))
		{
			size *= 2;
			cuv = realloc(cuv, size * sizeof(char));
		}
	}

	cuv[pass] = '\0';
	return cuv;
}

int main(int argc, char **argv)
{
	(void)argc; // nu folosesc argc, trebuia sa scap de warning-ul de la compile
	FILE *in = fopen(argv[1], "rt");
	FILE *out = fopen(argv[2], "wt");
	unsigned int n, i, error, longest_size = 0;
	char *op, *cuv;
	fscanf(in, "%u\n", &n);
	Trie *words = InitTrie();

	if (words == NULL)
	{
		fprintf(stderr, "Eroare alocare Trie!\n");
		fclose(in);
		fclose(out);
		return 1;
	}

	for (i = 0; i < n; i++)
	{
		char delim = ' ';
		op = strtok(get_word(in), &delim);
		cuv = strtok(NULL, &delim);

		if (strcmp(op, "add") == 0)
		{
			/*if (longest_size < strlen(cuv) + 1)
			{
				longest_size = strlen(cuv) + 1; // size cel mai lung sir
			}// neimplementat!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! waiting checker*/

			error = add(words, cuv);

			if (error != 0)
			{
				fprintf(stderr, "Eroare add pentru cuvantul: %s\n", cuv);
			}
		}

		if (strcmp(op, "remove") == 0)
		{
			remove_word(words, cuv);
		}

		if (strcmp(op, "find") == 0)
		{
			error = find(words, cuv);

			if (error == 0)
			{
				fprintf(out, "True\n");
			}
			else
			{
				fprintf(out, "False\n");
			}	
		}

		if (strcmp(op, "find_longest_prefix") == 0)
		{
			char *ans = find_longest_prefix(words, cuv);

			if (ans == NULL)
			{
				fprintf(out, "None\n");
			}
			else
			{
				fprintf(out, "%s\n", ans);
				free(ans);
			}
		}

		if (strcmp(op, "find_all_with_prefix") == 0)
		{
			find_all_with_prefix(words, cuv, out);
		}

		if (strcmp(op, "mean_length") == 0)
		{
			float math;
			math = mean_length(words, cuv);

			if (math == 0)
			{
				fprintf(out, "0\n");
			}
			else
			{
				fprintf(out, "%.3f\n", math);
			}
		}

		free(op);
	}

	DistTrie(&words);
	fclose(in);
	fclose(out);
	return 0;
}