#include "hash.h"

//Program realizat de Mindru Alexandru-Ionut
//Grupa 311CB

int main(void)
{
	FILE* in = fopen("input.in", "rt");
	FILE* out = fopen("output.out", "wt");

	int n, len, i;
	char op[9], type_key;
	fscanf(in, "%s %c %d\n%d", op, &type_key, &len, &n);
	Hash* HashTable = initHT(len, type_key);

	for (i = 0; i < n; i++)
	{
		fscanf(in, "%s", op);

		if (!strcmp(op, "insert"))
		{
			insertVal(HashTable, in);
		}
		else if (!strcmp(op, "print"))
		{
			HashTable->prtTH(HashTable, out);
		}
		else if (!strcmp(op, "delete"))
		{
			DistKey(HashTable, in);
		}
		else if (!strcmp(op, "find"))
		{
			findKey(HashTable, in, out);
		}
	}

	DistTH(&HashTable);
	fclose(in);
	fclose(out);
	return 0;
}