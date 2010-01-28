#include <stdio.h>
#include <stdlib.h>
#include "heap.h"

int main()
{
	int i, j;
	Heap h = HeapCreate(5);
	HeapInsert(h, 10.5, 1);
	for(j = 0; j < 5; j++) printf("%f ",h->values[j]); printf("\n");
	HeapInsert(h, 5.2, 2);
	for(j = 0; j < 5; j++) printf("%f ",h->values[j]); printf("\n");
	HeapInsert(h, 14.1, 3);
	for(j = 0; j < 5; j++) printf("%f ",h->values[j]); printf("\n");
	HeapInsert(h, 23.4, 4);
	for(j = 0; j < 5; j++) printf("%f ",h->values[j]); printf("\n");
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 5; j++)
		{
			printf("%f ",h->values[j]);
		}
		printf("\n%d: %f\n", HeapMaximumKey(h), HeapExtractMax(h));
	}
	return 0;
}