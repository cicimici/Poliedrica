typedef struct
{
	int *keys;
	int *revkeys;
	double *values;
	int size;
	int max_size;
} _heap;

typedef _heap* Heap;


void HeapInsert(Heap h, double value, int key);
void HeapIncreaseKey(Heap h, int i, double value);
void HeapIncreaseValue(Heap h, int key, double value);
void Heapify(Heap h, int i);
Heap HeapCreate(int n);
double HeapMaximumValue(Heap h);
double HeapExtractMax(Heap h);
int HeapMaximumKey(Heap h);
