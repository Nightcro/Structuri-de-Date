#include "common.h"

//Program realizat de Mindru Alexandru-Ionut
//Grupa 311CB

int main(int argc, char **argv)
{
	(void)argc; // nu folosesc argc, trebuia sa scap de warning-ul de la compile
	FILE *in = fopen(argv[1], "rt");
	FILE *out = fopen(argv[2], "wt");

	unsigned int error, i, n, nr_stive, nr_cozi, id;
	int ans;
	char op[9];
	fscanf(in, "%u %u %u", &n, &nr_stive, &nr_cozi);

	TStack *ListaS = initS(nr_stive, PrintVal);
	
	if ((ListaS->bucket == NULL) && (nr_stive != 0))
	{
		fprintf(stderr, "Eroare alocare stive\n");
		return 1;
	}

	TQueue *ListaQ = initQ(nr_cozi, PrintVal);

	if ((ListaQ->bucket == NULL) && (nr_cozi != 0))
	{
		free(ListaS);
		fprintf(stderr, "Eroare alocare cozi\n");
		return 1;
	}

	for (i = 0; i < n; i++)
	{
		fscanf(in, "%s", op);
		
		if (strcmp(op, "push") == 0)
		{
			error = ReadVal(ListaS, in, op);

			if (error == 1)
			{
				fprintf(stderr, "Eroare alocare informatie\n");
			}
			else if (error == 2)
			{
				fprintf(stderr, "Eroare push\n");
			}
		}

		if (strcmp(op, "pop") == 0)
		{
			void *aux;
			fscanf(in, "%u", &id);
			error = pop(&ListaS->bucket[id], &aux);
			
			if (error == 0) // daca nu e nicio problema
			{			// insemana ca avem o adresa pe care putem sa o eliberam
				((Info*)aux)->fr(((Info*)aux)->val);
				free(aux);
			}
		}
		
		if (strcmp(op, "prints") == 0)
		{
			error = prints(ListaS, out);

			if (error == 1)
			{
				fprintf(stderr, "Eroare afisare stive\n");
			}
		}

		if (strcmp(op, "sorts") == 0)
		{
			fscanf(in, "%u", &id); // trimit eroarea 0 (explicat la functie)
			error = sorts(&ListaS->bucket[id], cmp_brackets_id, 0);

			if (error == 1)
			{
				fprintf(stderr, "Eroare sortare Stack\n");
			}
		}

		if (strcmp(op, "corrects") == 0)
		{
			fscanf(in, "%u", &id);
			error = corrects(&ListaS->bucket[id], cmp_brackets_type, &ans);
			fprintf(out, "%d\n", ans);

			if (error == 1)
			{
				fprintf(stderr, "Eroare corrects\n");
			}
		}

		if (strcmp(op, "intrq") == 0)
		{
			error = ReadVal(ListaQ, in, op);

			if (error == 1)
			{
				fprintf(stderr, "Eroare alocare informatie\n");
			}
			else if (error == 2)
			{
				fprintf(stderr, "Eroare intrq\n");
			}
		}

		if (strcmp(op, "extrq") == 0)
		{
			
			void *aux;
			fscanf(in, "%u", &id);
			error = extrq(&ListaQ->bucket[id], &aux);
			
			if (error == 0) // daca nu e nicio problema
			{			// insemana ca avem o adresa pe care putem sa o eliberam
				((Info*)aux)->fr(((Info*)aux)->val);
				free(aux);
			}
		}

		if (strcmp(op, "sortq") == 0)
		{
			fscanf(in, "%u", &id); // trimit eroarea 0 (explicat la functie)
			error = sortq(&ListaQ->bucket[id], cmp_brackets_id, 0);

			if (error == 1)
			{
				fprintf(stderr, "Eroare sortare coada\n");
			}
		}

		if (strcmp(op, "printq") == 0)
		{
			error = printq(ListaQ, out);

			if (error == 1)
			{
				fprintf(stderr, "Eroare afisare cozi\n");
			}
		}

		if (strcmp(op, "correctq") == 0)
		{
			fscanf(in, "%u", &id);
			error = correctq(&ListaQ->bucket[id], cmp_brackets_type, &ans);
			fprintf(out, "%d\n", ans);

			if (error == 1)
			{
				fprintf(stderr, "Eroare correctq\n");
			}
		}
	}

	DistS(&ListaS);
	DistQ(&ListaQ);
	fclose(in);
	fclose(out);
	return 0;
}