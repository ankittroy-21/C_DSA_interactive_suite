#ifndef GRAPH_IO_H
#define GRAPH_IO_H
#include "graph_traversals.h"

// Loads a directed weighted graph from a CSV file for the graph algorithms.
//
// Expected format (Dijkstra):
//   line 1 : <number_of_vertices> <number_of_edges>
//   next E : <src>,<dest>,<weight>          (one directed edge per line)
//
// Returns a newly allocated weightedGraph on success (caller frees it with
// free_weightedGraph), or NULL if the file cannot be opened or its contents
// are malformed/out of range. A descriptive message is printed on failure.
weightedGraph* load_weightedGraph_from_csv(const char* path);

// Loads a directed weighted graph together with a per-vertex heuristic from a
// CSV file, for the informed searches (A* / Greedy Best-First).
//
// Expected format:
//   line 1 : <number_of_vertices> <number_of_edges>
//   next E : <src>,<dest>,<weight>          (one directed edge per line)
//   last   : <h0> <h1> ... <h(V-1)>         (one non-negative heuristic / vertex)
//
// The edge section is identical to load_weightedGraph_from_csv; the only extra
// is the trailing line of V heuristic values.
//
// On success returns a newly allocated weightedGraph (caller frees it with
// free_weightedGraph) and stores a newly allocated int[V] of heuristics in
// *out_h (caller frees it with free). On failure returns NULL, sets *out_h to
// NULL, and prints a descriptive message.
weightedGraph* load_weightedGraph_with_heuristic_from_csv(const char* path, int** out_h);

// Loads an unweighted, undirected graph from a CSV file for the traversals
// (BFS / DFS).
//
// Expected format:
//   line 1 : <number_of_vertices> <number_of_edges>
//   next E : <src>,<dest>                    (one undirected edge per line)
//
// Each edge is added with add_edge_undirected, so both endpoints reference each
// other. Returns a newly allocated Graph on success (caller frees it with
// free_graph), or NULL if the file cannot be opened or its contents are
// malformed/out of range. A descriptive message is printed on failure.
Graph* load_graph_from_csv(const char* path);

#endif
