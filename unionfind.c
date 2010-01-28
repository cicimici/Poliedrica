#include "unionfind.h"

int FindSet(UF uf, int x)
{
	if(x != uf->parent[x])
		uf->parent[x] = findset(uf->parent[x]);
	return uf->parent[x];
}

void Union(UF uf, int x, int y)
{
	int xp, yp;
	xp = findset(x);
	yp = findset(y);
	if(uf->rank[xp] > uf->rank[yp])
		uf->parent[yp] = xp;
	else
	{
		uf->parent[xp] = yp;
		if(uf->rank[xp] == uf->rank[yp])
			uf->rank[yp]++;
	}
}

void MakeSet(UF uf, int x)
{
	uf->parent[x] = x;
	uf->rank[x] = 0;
}

UF CreateUF(int size)
{
	UF uf = (UF)malloc(sizeof(_uf));
	size++;
	uf->parent = (int *)malloc(size* sizeof(int));
	uf->rank = (int *)malloc(size* sizeof(int));
	return uf;
}