#include "gb_graph.h"
#include "gb_plane.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int random_i(int maxW)
{
  return (int) (maxW * (rand() / (RAND_MAX + 1.0)));
}

void print_usage(char *name)
{
	printf("%s n l prob maxW [seed]\n", name);
	printf("n - number of Vertex\n");
	printf("l - square size (must be between 0 and 16384)\n");
	printf("prob - Probability for removing an edge (must be between 0 and 65536)\n");
	printf("maxW - maximum weight on edges\n");
	printf("seed - seed for srand\n");
	exit(-1);
}


int main(int argc, char *argv[])
{
  int d, n, l, prob, maxW;
  Graph *g;
  Arc *a;
  Vertex *v;
  if(argc < 5)
    print_usage(argv[0]);
  if(argc == 6)
    srand(atoi(argv[5]));
  else
    srand(time(0));
  n = atoi(argv[1]);		
  l = atoi(argv[2]);
  prob = atoi(argv[3]);
  maxW = atoi(argv[4]);
  if(n < 0 || l < 0 || l > 16384 || prob < 0 || prob > 65536 || maxW < 0)
    print_usage(argv[0]);
	
  g = plane(n,l,l,0,prob,random_i((1<<30) - 1));
  printf("%ld %ld U\n",g->n,g->m/2);
  
  for(v = g->vertices; v < g->vertices + g->n; v++)
    {
      d = 0;
      for (a = v->arcs ; a; a = a->next) 
	d++;
      printf("%d 0 0 0\n", d);
      for (a = v->arcs ; a; a = a->next) 
	printf("%d %d\n", atoi(a->tip->name) + 1, random_i(maxW));
    }
  return 0;
}
