#include "hash.h"
#include "materie.h"

//Program realizat de Mindru Alexandru-Ionut
//Grupa 311CB

TMaterie* readMat(FILE* in)
{
	TMaterie* temp = malloc(sizeof(TMaterie));
	char back_materie[50];
	fscanf(in, "%s %d %d ", back_materie, &temp->ore_curs, &temp->ore_lab);
	fscanf(in, "%d %d", &temp->credit, &temp->teme);
	temp->materie = malloc((strlen(back_materie) + 1) * sizeof(char));
	// citesc intr-o valoare statica pentru a aloca dinamic cat mai eficient
	if (temp->materie == NULL)
	{
		return NULL;
	}

	memcpy(temp->materie, back_materie, (strlen(back_materie) + 1));
	return temp;
}

void printMat(void* temp, FILE* out)
{
	TMaterie* aux = (TMaterie*)temp;
	fprintf(out, "[Materie: %s, Ore_curs: %d, ", aux->materie, aux->ore_curs);
	fprintf(out, "Ore_lab: %d, Credit: %d, Teme: %d]", 
			aux->ore_lab, aux->credit, aux->teme);
	return;
}

void freeMat(void* temp)
{
	free(((TMaterie*)temp)->materie);
	return;
}