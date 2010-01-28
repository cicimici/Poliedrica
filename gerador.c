#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

typedef struct _node
{
	int v, w;
	struct _node *next;
} node;

typedef node* link;

typedef struct
{
	double x, y;
} coord;

typedef struct
{
	int V,E;
	link *adj;
	coord *c;
} graph;

graph *create_graph(int n)
{
	int i;
	graph *g = (graph *)malloc(sizeof(graph));
	g->V = n;
	g->E = 0;
	g->adj = (link *)malloc(n*sizeof(link));
	for(i = 0; i < n; i++)
		g->adj[i] = NULL;
	g->c = (coord *)malloc(n*sizeof(coord));
	return g;
}

void print_usage(char *name)
{
	printf("%s n d l maxW [seed]\n", name);
	printf("n - number of Vertex\n");
	printf("d - maximum distance to a edge be included\n");
	printf("l - square size\n");
	printf("maxW - maximum weight on edges\n");
	printf("seed - seed for srand\n");
	exit(-1);
}

double random_f(int l)
{
	return (l * (rand() / (RAND_MAX + 1.0)));
}

int random_i(int maxW)
{
	return (int) (maxW * (rand() / (RAND_MAX + 1.0)));
}

link NEW(int v, int w, link next)
{
	link p = (link)malloc(sizeof(node));
	p->v = v;
	p->w = w;
	p->next = next;
	return p;
}

void insere(graph *g, int u, int v, int w)
{
	g->adj[u] = NEW(v,w,g->adj[u]);
	g->adj[v] = NEW(u,w,g->adj[v]);
	(g->E)++;
}

int dist(coord a, coord b, int d)
{
	return ((a.x*b.x + a.y*b.y) <= (d*d));
}

double determinante(coord a, coord b, coord c)
{
	return a.x*(b.y - c.y) + b.x*(c.y - a.y) + c.x*(a.y - b.y);
}

int intercepta(graph *g, int n, int v, coord a, coord b)
{
	int i;
	link p;
	coord c, d;
	double s1, s2;
	for(i = 0; i < n; i++)
	{
		if(i == v)
			continue;
		for(p = g->adj[i]; p != NULL; p = p->next)
		{
			if(i == p->v)
				continue;			
			c = g->c[i];
			d = g->c[p->v];
			
			s1 = determinante(a,b,c);
			s2 = determinante(a,b,d);
			if(s1*s2 >= 0)
				continue;
			s1 = determinante(c,d,a);
			s2 = determinante(c,d,b);
			if(s1*s2 >= 0)
				continue;
			return 1;
		}
	}
	return 0;
}

void gera_pontos(graph *g, int d, int l, int maxW)
{
	int i,j;
	for(i = 0; i < g->V; i++)
	{
		/*define as coordenadas*/
		g->c[i].x = random_f(l);
		g->c[i].y = random_f(l);
		/*procura por pontos no raio*/
		for(j = 0; j < i; j++)
		{
			if(dist(g->c[i],g->c[j], d) && !intercepta(g, i, j, g->c[i], g->c[j]))
				insere(g,i,j, random_i(maxW));
		}
	}
}

void imprime(graph *g)
{
	int i, d;
	link p;
	printf("%d %d U\n",g->V, g->E);
	for(i = 0; i < g->V; i++)
	{
		d = 0;
		for(p = g->adj[i]; p != NULL; p = p->next) d++;
		printf("%d %d 0 0\n", d, i+1);
		for(p = g->adj[i]; p != NULL; p = p->next)
			printf("%d %d\n", p->v + 1, p->w);
	}
}

int main(int argc, char *argv[])
{
	int n, d, l, maxW;
	graph *g;
	if(argc < 5)
		print_usage(argv[0]);
	if(argc == 6)
		srand(atoi(argv[5]));
	else
		srand(time(0));
		
	n = atoi(argv[1]);	
	d = atoi(argv[2]);	
	l = atoi(argv[3]);	
	maxW = atoi(argv[4]);	
	
	g = create_graph(n);
	gera_pontos(g,d,l,maxW);
	imprime(g);
	return 0;
}
