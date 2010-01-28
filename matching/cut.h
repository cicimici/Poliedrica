#include "matching.h"

graph_t *create_cut_graph(glp_prob *lp, graph_t *graph);
void update_cut_graph(glp_prob *lp, graph_t *cut_graph);
double min_cut(graph_t *graph, list_t *cut_set);
double min_cut_phase(graph_t *graph,
