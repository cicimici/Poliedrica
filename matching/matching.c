#include "matching.h"

int global_ncuts;
glp_tree *global_tree;

int main(int argc, char **argv)
{
  graph_t *graph;
  char input_fname[256];

  if (argc <= 1) {
    printf("Error: Input file unspecified.\n");
    exit(EXIT_FAILURE);
  }

  strcpy(input_fname, argv[1]);

  graph = read_own_format(input_fname);

  /*glp_term_out(GLP_OFF);*/

  solver(graph);

  free_graph(graph);

  return 0;
}

graph_t *read_wmatch(char *fname)
{
  int i, j, v, e, d, u;
  double w;
  FILE *input;
  graph_t *graph;

  input = fopen(fname, "r");
  if (input == NULL) {
    fprintf(stderr, "Error: Unable to open %s.\n", fname);
    exit(EXIT_FAILURE);
  }

  fscanf(input, "%d %d", &v, &e);
  graph = create_graph(v);

  for (i = 1; i <= v; i++) {
    fscanf(input, "%d %*d %*d %*d", &d);
    for (j = 1; j <= d; j++) {
      fscanf(input, "%d %lf", &u, &w);
      insert_edge(graph, i, u, w);
    }
  }

  return graph;
}

graph_t *read_own_format(char *fname)
{
  int i, j, k, v, e;
  double w;
  FILE *input;
  graph_t *graph;

  input = fopen(fname, "r");
  if (input == NULL) {
    fprintf(stderr, "Error: Unable to open %s.\n", fname);
    exit(EXIT_FAILURE);
  }

  fscanf(input, "%d %d", &v, &e);
  graph = create_graph(v);

  for (i = 1; i <= e; i++) {
    fscanf(input, "%d %d %lf", &j, &k, &w);
    insert_edge(graph, j, k, w);
    insert_edge(graph, k, j, w);
  }

  return graph;
}

list_t *create_list(int v, double w, list_t *next)
{
  list_t *l;

  l = (list_t *) malloc_chk(sizeof (list_t));
  l->v = v;
  l->w = w;
  l->next = next;

  return l;
}

list_t *copy_list(list_t *list)
{
  list_t *copy;

  copy = NULL;

  if (list != NULL)
    copy = create_list(list->v, list->w, copy_list(list->next));

  return copy;
}

void free_list(list_t *list)
{
  if (list != NULL) {
    free_list(list->next);
    free(list);
  }
}

list_t **create_adj(int v)
{
  int i;
  list_t **adj;

  adj = (list_t **) malloc_chk((v + 1) * sizeof (list_t *));
  for (i = 0; i <= v; i++)
    adj[i] = NULL;

  return adj;
}

graph_t *create_graph(int v)
{
  graph_t *graph;

  graph = (graph_t *) malloc_chk(sizeof (graph_t));
  graph->adj = create_adj(v);
  graph->v = v;
  graph->e = 0;

  return graph;
}

void free_graph(graph_t *graph)
{
  int i;

  for (i = 1; i <= graph->v; i++)
    free_list(graph->adj[i]);

  free(graph);
}

graph_t *copy_graph(graph_t *graph, int extra_vertices)
{
  int i;
  list_t *p;
  graph_t *copy;

  copy = create_graph(graph->v + extra_vertices);
  for (i = 1; i <= graph->v; i++)
    for (p = graph->adj[i]; p != NULL; p = p->next)
      insert_edge(copy, i, p->v, p->w);

  return copy;
}

void insert_edge(graph_t *graph, int u, int v, double w)
{
  graph->adj[u] = create_list(v, w, graph->adj[u]);
  /*graph->adj[v] = create_list(u, w, graph->adj[v]);*/
  (graph->e)++;
}

void *malloc_chk (unsigned int n)
{
  void *ptr;
  if ((ptr = malloc(n)) == NULL) {
    fprintf(stderr, "Error: Unable to allocate memory.\n");
    exit(EXIT_FAILURE);
  }
  return ptr;
}

void solver(graph_t *graph)
{
  struct tms t_start, t_end;
  double time_elapsed;
  glp_prob *lp;
  glp_smcp *lpparm;
  glp_iocp *ipparm;

  times(&t_start);

  global_ncuts = 0;

  lp = build_lp(graph);

  lpparm = (glp_smcp *) malloc_chk(sizeof (glp_smcp));
  ipparm = (glp_iocp *) malloc_chk(sizeof (glp_iocp));

  glp_init_smcp(lpparm);
  glp_init_iocp(ipparm);

  ipparm->cb_func = callback_bb;
  ipparm->cb_info = graph;

  glp_simplex(lp, lpparm);
  glp_intopt(lp, ipparm);

  times(&t_end);
  time_elapsed = (((t_end.tms_utime + t_end.tms_cutime) -
		   (t_start.tms_utime + t_start.tms_cutime)) /
                  sysconf(_SC_CLK_TCK));

  /*print_solution(lp, graph);*/
  print_info(lp, graph, time_elapsed);

  glp_delete_prob(lp);
}

glp_prob *build_lp(graph_t *graph)
{
  int i, len;
  int *ind;
  double *val;
  list_t *p;
  glp_prob *lp;

  printf("Building model...\n");

  ind = (int *) malloc_chk(graph->e * sizeof (int));
  val = (double *) malloc_chk(graph->e * sizeof (double));

  /* create problem and set objective function direction */
  lp = glp_create_prob();
  glp_set_prob_name(lp, "matching");
  glp_set_obj_name(lp, "obj");
  glp_set_obj_dir(lp, GLP_MAX);

  /* fix a LP variable for each edge */
  set_lp_vars(graph);

  /* add |V| rows and |E| cols */
  glp_add_rows(lp, graph->v);
  glp_add_cols(lp, graph->e);

  /* set variables to binary */
  for (i = 1; i <= graph->e; i++)
    glp_set_col_kind(lp, i, GLP_BV);

  /* set degree constraints */
  for (i = 1; i <= graph->v; i++) {
    len = 0;
    for (p = graph->adj[i]; p != NULL; p = p->next) {
      len++;
      ind[len] = p->lp_var;
      val[len] = 1;
    }
    glp_set_mat_row(lp, i, len, ind, val);
    glp_set_row_bnds(lp, i, GLP_UP, 0, 1);
  }

  /* set weights */
  for (i = 1; i <= graph->v; i++)
    for (p = graph->adj[i]; p != NULL; p = p->next)
      if (i < p->v)
	glp_set_obj_coef(lp, p->lp_var, p->w);

  free(ind);
  free(val);

  printf("Model has been successfully generated\n");

  return lp;
}

void callback_bb(glp_tree *tree, void *info)
{
  int ncuts;
  graph_t *graph;
  glp_prob *lp;

  graph = (graph_t *) info;

  /* Horrible. I don't know how to get the tree for print_info. */
  global_tree = tree; 

  switch (glp_ios_reason(tree)) {
  case GLP_ICUTGEN:
    lp = glp_ios_get_prob(tree);
    ncuts = cut_odd_component(lp, graph, 0);
    if (ncuts == 0)
      ncuts += cut_odd_component(lp, graph, 0.3);
    global_ncuts += ncuts;
    break;
  }
}

int *vars_to_matching(glp_prob *lp, graph_t *graph)
{
  int i;
  int *matching;
  list_t *p;

  matching = (int *) malloc_chk((graph->v + 1) * sizeof (int));
  for (i = 1; i <= graph->v; i++)
    matching[i] = 0;

  for (i = 1; i <= graph->v; i++) {
    for (p = graph->adj[i]; p != NULL; p = p->next) { 
      if (i < p->v) {
	if ((int) (glp_get_col_prim(lp, p->lp_var) + EPSILON) == 1) {
	  if (matching[i] != 0) 	/* TODO: Debug. Fix me. */
	    fprintf(stderr, "Error: Two edges incident to a same vertex.\n");
	  matching[i] = p->v;
	  matching[p->v] = i;
	  /* break; */
	}
      }
    }
  }

  return matching;
}

/* Finds odd components (considering only edges with lp values greater
 * than eps), builds and inserts the appropriate constraints, and returns
 * the number of such cuts.
 */
int cut_odd_component(glp_prob *lp, graph_t *graph, double eps)
{
  int i, j, k, row, rep_i, rep_v, ncuts;
  int *comp_count, *ind;
  double *val, sum;
  list_t *p;
  UF comp;

  comp_count = (int *) malloc_chk((graph->v + 1) * sizeof (int));
  comp = CreateUF(graph->v);

  for (i = 1; i <= graph->v; i++) {
    MakeSet(comp, i);
    comp_count[i] = 1;
  }

  for (i = 1; i <= graph->v; i++) {
    for (p = graph->adj[i]; p != NULL; p = p->next)
      if (i < p->v)
	if (glp_get_col_prim(lp, p->lp_var) + EPSILON > eps) {
	  rep_i = FindSet(comp, i);
	  rep_v = FindSet(comp, p->v);
	  if (rep_i != rep_v) {
	    Union(comp, rep_i, rep_v);
	    if (FindSet(comp, rep_i) == rep_i)
	      comp_count[rep_i] += comp_count[rep_v];
	    else
	      comp_count[rep_v] += comp_count[rep_i];
	  }
	}
  }

  ind = (int *) malloc_chk((graph->e + 1) * sizeof (int));
  val = (double *) malloc_chk((graph->e + 1) * sizeof (double));
  ncuts = 0;

  for (i = 1; i <= graph->v; i++) {
    if (comp_count[i] >= 3 && comp_count[i] % 2 == 1 &&
	FindSet(comp, i) == i) {
      k = 0;
      sum = 0;
      for (j = 1; j <= graph->v; j++) {
	if (FindSet(comp, j) == i) {
	  for (p = graph->adj[j]; p != NULL; p = p->next)
	    if (j < p->v && FindSet(comp, p->v) == i) {
	      k++;
	      ind[k] = p->lp_var;
	      val[k] = 1;
	      sum += glp_get_col_prim(lp, p->lp_var);
	    }
	}
      }
      if (sum - EPSILON > (comp_count[i] - 1) / 2) {
	glp_add_rows(lp, 1);
	row = glp_get_num_rows(lp);
	glp_set_mat_row(lp, row, k, ind, val);
	glp_set_row_bnds(lp, row, GLP_UP, 0, (comp_count[i] - 1) / 2);
	ncuts++;
	/*printf("Debug: cut added (%f > %d).\n", sum, (comp_count[i] - 1) / 2);*/
      }
    }
  }

  free(ind);
  free(val);

  return ncuts;
}

void set_lp_vars(graph_t *graph)
{
  int i, k;
  list_t *p, *q;

  k = 1;
  for (i = 1; i <= graph->v; i++) {
    for (p = graph->adj[i]; p != NULL; p = p->next) { 
      if (i < p->v)
	p->lp_var = k++;
      else {
	for (q = graph->adj[p->v]; q != NULL; q = q->next)
	  if (q->v == i) {
	    p->lp_var = q->lp_var;
	    break;
	  }
      }
    }
  }
}

void print_solution(glp_prob *lp, graph_t *graph)
{
  int i;
  int *matching;

  matching = vars_to_matching(lp, graph);

  for (i = 1; i <= graph->v; i++)
    printf("%d %d\n", i, matching[i]);

  printf("\nSolution: %d\n", (int) (glp_mip_obj_val(lp) + EPSILON));
}

void print_info(glp_prob *lp, graph_t *graph, double time)
{
  int tnodes;

  glp_ios_tree_size(global_tree, NULL, NULL, &tnodes);

  /* |V| |E| solution time ncuts total_nodes */

  printf("\n");
  printf("# vertices:\t%d\n", graph->v);
  printf("# edges (vars):\t%d\n", graph->e);
  printf("Solution:\t%d\n", (int) (glp_mip_obj_val(lp)+EPSILON));
  printf("Time elapsed:\t%f\n", time);
  printf("# total cuts:\t%d\n", global_ncuts);
  printf("# total nodes:\t%d\n", tnodes);
}
