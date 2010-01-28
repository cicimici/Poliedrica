typedef struct 
{
	int *parent;
	int *rank;
} _uf;

typedef _uf* UF;

int FindSet(UF uf, int x);
void Union(UF uf, int x, int y);
void MakeSet(UF uf, int x);
UF CreateUF(int size);