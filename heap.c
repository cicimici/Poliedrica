#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include "heap.h"
#define PARENT(i) ((i-1)/2)
#define LEFT(i) (2*i + 1)
#define RIGHT(i) (2*i + 2)

void exchange(Heap h, int i, int j)
{
	double temp;
	temp = h->values[i];
	h->values[i] = h->values[j];
	h->values[j] = temp;
	h->keys[h->revkeys[i]] = j;
	h->keys[h->revkeys[j]] = i;
	/*atualizar*/
	temp = h->revkeys[i];
	h->revkeys[i] = h->revkeys[j];
	h->revkeys[j] = temp;
}

Heap HeapCreate(int n)
{
	int i;
	Heap h = (Heap)malloc(sizeof(_heap));
	h->keys = (int *)malloc((n+1)*sizeof(int));
	h->revkeys = (int *)malloc((n+1)*sizeof(int));
	for(i = 0; i <= n; i++)
		h->keys[i] = h->revkeys[i] = -1;
	h->values = (double *)malloc(n*sizeof(double));
	h->size = 0;
	h->max_size = n;
	return h;
}

double HeapMaximumValue(Heap h)
{
	return h->values[0];
}

int HeapMaximumKey(Heap h)
{
	return h->revkeys[0];
}

void HeapIncreaseValue(Heap h, int key, double value)
{
  int v = h->keys[key];
  if(value > 0)
    HeapIncreaseKey(h, v, h->values[v] + value);
  else
    {
      h->values[v] += value;
      Heapify(h,v);
    }
}

double HeapExtractMax(Heap h)
{
	double max;
	if(h->size == 0)
		{	
			fprintf(stderr, "Heap underflow\n");
			exit(-1);
		}
	max = h->values[0];	
	/*indica que a chave não estará mais no heap*/
	h->keys[h->revkeys[0]] = -1;
	exchange(h, 0, h->size - 1);
	(h->size)--;
	Heapify(h,0);
	return max;
}

void Heapify(Heap h, int i)
{
	int l, r, largest;
	double temp;
	l = LEFT(i);
	r = RIGHT(i);
	if(l < h->size && h->values[l] > h->values[i])
		largest = l;
	else
		largest = i;
	if(r < h->size && h->values[r] > h->values[largest])
		largest = r;
	if(largest != i)
	{
		exchange(h,i,largest);
		Heapify(h,largest);
	}	
}

void HeapIncreaseKey(Heap h, int i, double value)
{
	double temp;
	if(value < h->values[i])
		fprintf(stderr, "New key os smaller than current key\n");
	h->values[i] = value;
	while(i > 0 && h->values[PARENT(i)] < h->values[i])
	{
		exchange(h, i, PARENT(i));
		i = PARENT(i);
	}
}

void HeapInsert(Heap h, double value, int key)
{
	h->values[h->size] = -1;
	h->keys[key] = h->size;
	h->revkeys[h->size] = key;
	(h->size)++;
	HeapIncreaseKey(h,h->size-1,value);
}
