/* Isso é parte do callback_bb */

/*     cut = (int *) malloc_chk((graph->e + 1) * sizeof (int)); */

/*     lp = glp_ios_get_prob(tree); */
/*     cut_weight = min_cut(create_cut_graph(lp, graph), cut_set); */
    
/*     /\* cut vetor de arestas do corte *\/ */
/*     /\* cut_part cardinalidade da partição *\/ */
/*     /\* cut_size cardinalidade do corte *\/ */
/*     if (cut_weight < 1.0 - EPSILON) { */
/*       glp_add_rows(lp, 1); */
/*       row = glp_get_num_rows(lp); */
/*       val = (double *) malloc_chk((cut_size + 1) * sizeof (double)); */
/*       for (i = 1; i <= cut_size; i++) */
/* 	val[i] = 1; */
/*       glp_set_mat_row(lp, row, cut_weight, cut, val); */
/*       glp_set_row_bnds(lp, row, GLP_UP, 0, */
/* 		       ceil((cut_part - 1 / 2.0) - EPSILON)); */
/*       free(val); */
/*     } */

/*     free(cut); */

/* This graph has the following structure:
 * - vertices as original graph, plus vertex v+1
 * - edges as original graph, plus edges between all vertices and v+1
 * - weights as current solution for original edges
 * - weights as degree constraint slacks for edges between a vertex and v+1
 * - (important) the first node of each list is always v+1 (except v+1 itself)
 * - the edges of vertex v+1 are in increasing order in the list
 */
graph_t *create_cut_graph(glp_prob *lp, graph_t *graph)
{
  int i;
  graph_t *cut_graph;

  cut_graph = copy_graph(graph, 1);
  for (i = cut_graph->v - 1; i >= 1; i--) {
    insert_edge(cut_graph, cut_graph->v, i, 0);
    insert_edge(cut_graph, i, cut_graph->v, 0);
  }
  update_cut_graph(lp, cut_graph);

  return cut_graph;
}

/* Supposes first node of the list for each vertex v is edge v-0 and the
 * node list for vertex 0 is in increasing order.
 */
void update_cut_graph(glp_prob *lp, graph_t *cut_graph)
{
  int i, k;
  double deg_sum;
  list_t *p, *q;

  k = 1;
  q = cut_graph->adj[cut_graph->v];
  for (i = 1; i < cut_graph->v; i++) {
    for (p = cut_graph->adj[i]->next; p != NULL; p = p->next) {
      if (i < p->v) {
	p->w = glp_get_col_prim(lp, k++);
	deg_sum += p->w;
      }
    }
    cut_graph->adj[i]->w = q->w = 1 - deg_sum;
    q = q->next;
  }
}

/* Destroys graph. */
double min_cut(graph_t *graph, list_t *cut_set)
{
  int i, j, before_last, last;
  double mincut, cut;
  UF partition;

  cut_set = NULL;
  partition = CreateUF(graph->v);
  for (i = 1; i <= graph->v; i++)
    MakeSet(partition, i);

  mincut = DBL_MAX;
  while (graph->v > 1) {
    cut = min_cut_phase(graph, partition, &before_last, &last);
    if (cut < mincut) {
      mincut = cut;
      free_list(cut_set);
      cut_set = NULL;
      for (j = 1; j <= graph->v; j++)
	if (FindSet(partition, j) == last)
	  cut_set = create_list(j, 0, cut_set);
    }
    Union(partition, before_last, last);
  }

  return mincut;
}

double min_cut_phase(graph_t *graph, UF partition, int *before_last,
		     int *last)
{
  int i, j, v, heap_size, repw;
  list_t *p;
  Heap heap;

  /* first vertex is graph->v (w*) */
  repw = FindSet(partition, graph->v);
  heap = HeapCreate(graph->v - 1);
  for (i = 1; i <= graph->v - 1; i++)
    if (i == FindSet(partition, i) && i != repw)
      HeapInsert(heap, i, 0);

  for (j = 1; j <= graph->v; j++)
    if (FindSet(partition, j) == repw)
      for (p = graph->adj[j]; p != NULL; p = p->next)
	HeapIncreaseValue(heap, FindSet(partition, p->v), p->w);
  heap_size = heap->size;

  *before_last = 1;
  for (i = 1; i <= heap_size - 1; i++) {
    v = HeapMaximumKey(heap);
    HeapExtractMax(heap);
    for (j = 1; j <= graph->v; j++)
      if (FindSet(partition, j) == v)
	for (p = graph->adj[j]; p != NULL; p = p->next)
	  HeapIncreaseValue(heap, FindSet(partition, p->v), p->w);
    *before_last = v;
  }

  *last = HeapMaximumKey(heap);

  return HeapExtractMax(heap);
}

