#include "graphtypes.h"

#include <sys/times.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

main(argc,argv)
int argc;
char *argv[];

{
	Graph graph;
	int *Mate;
	int i,size;
	/*Used to find the cost of the matching*/
	int c = 0;
	Edge e;
	struct tms t_start, t_end;
	double time_elapsed;

	if (argc < 2){
		printf("Usage: wmatch datafile\n");
		exit(1);
	}
	graph = ReadGraph(&size,argv[1]);

	times(&t_start);
	Mate = Weighted_Match(graph,1);
	times(&t_end);
	time_elapsed = (((t_end.tms_utime + t_end.tms_cutime) -
			 (t_start.tms_utime + t_start.tms_cutime)) /
			sysconf(_SC_CLK_TCK));

	for (i=1; i<=size; i++)
	  {
	    /*add the cost of edge i->Mate[i] only once*/
	    if(i < Mate[i])
	      {
		/*find Mate[i] on i adj_list*/
		for(e = graph[i].adj_list; e != NULL && e->endpoint != Mate[i]; e = e->nextedge);
		/*if e == NULL then the program is wrong.*/
		if(e != NULL)
		  c += e->label;
	      }
	    /*printf("%d %d\n",i,Mate[i]);*/
	  }
	printf("\n%d\n",c);
	printf("%f\n", time_elapsed);
}


