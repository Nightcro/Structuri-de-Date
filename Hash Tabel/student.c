#include "hash.h"
#include "student.h"

//Program realizat de Mindru Alexandru-Ionut
//Grupa 311CB

TStudent* readStud(FILE* in)
{
	TStudent* temp = malloc(sizeof(TStudent));
	char back_nume[50];
	fscanf(in, "%s %f", back_nume, &temp->medie);
	fscanf(in, "%s %d", temp->grupa, &temp->varsta);
	temp->nume = malloc((strlen(back_nume) + 1) * sizeof(char));
	// citesc intr-o valoare statica pentru a aloca dinamic cat mai eficient
	if (temp->nume == NULL)
	{
		return NULL;
	}

	memcpy(temp->nume, back_nume, (strlen(back_nume) + 1));
	return temp;
}

void printStud(void* temp, FILE* out)
{
	TStudent* aux = (TStudent*)temp;
	fprintf(out, "[Nume: %s, Grupa: %s, Medie: %.2f, Varsta: %d]", 
			aux->nume, aux->grupa, aux->medie, aux->varsta);
	return;
}

void freeStud(void* temp)
{
	free(((TStudent*)temp)->nume);
	return;
}