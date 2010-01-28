#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <sys/times.h>
#include <unistd.h>

#include "glpk.h"
#include "heap.h"
#include "unionfind.h"

#define EPSILON 1e-7

typedef struct list list_t;
typedef struct graph graph_t;

struct list {
  int v;
  double w;
  int lp_var;
  list_t *next;
};

struct graph {
  list_t **adj;
  int v;
  int e;
};

graph_t *read_wmatch(char *fname);
graph_t *read_own_format(char *fname);
list_t *create_list(int v, double w, list_t *next);
list_t *copy_list(list_t *list);
void free_list(list_t *list);
list_t **create_adj(int v);
graph_t *create_graph(int v);
void free_graph(graph_t *graph);
void insert_edge(graph_t *graph, int u, int v, double w);
void *malloc_chk (unsigned int n);
void construct_data(graph_t *graph);
void solver(graph_t *graph);
glp_prob *build_lp(graph_t *graph);
void callback_bb(glp_tree *tree, void *info);
graph_t *create_cut_graph(glp_prob *lp, graph_t *graph);
void update_cut_graph(glp_prob *lp, graph_t *cut_graph);
double min_cut(graph_t *graph, list_t *cut_set);
double min_cut_phase(graph_t *graph, UF partition, int *before_last,
		     int *last);
int *vars_to_matching(glp_prob *lp, graph_t *graph);
int cut_odd_component(glp_prob *lp, graph_t *graph, double eps);
void set_lp_vars(graph_t *graph);
void print_solution(glp_prob *lp, graph_t *graph);
void print_info(glp_prob *lp, graph_t *graph, double time);
