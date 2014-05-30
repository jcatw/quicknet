#ifndef QUICKNET_NETWORKNODE_H
#define QUICKNET_NETWORKNODE_H

#include "types.h"

struct directed_node {
  uint64_t id;
  uint64_t in_degree;
  uint64_t out_degree;
  double lambda;
  double mu;
  int64_t tarjindex;   //used for scc search
  int64_t tarjlowlink; //this too
  char on_stack;        //also this
  directed_node_list_t *adjacency_list;
};

struct directed_node_list {
  directed_node_t *node;
  directed_node_list_t *next;
};

directed_node_t *make_directed_node(uint64_t id, double lambda, double mu);
directed_node_t *clone_directed_node(directed_node_t *node);
directed_node_list_t *make_directed_node_list(directed_node_t *n);
void add_directed_edge(directed_node_t *n_from, directed_node_t *n_to);

void directed_node_free(directed_node_t *node, char free_adjacency_list);
void directed_node_list_free(directed_node_list_t *node_list);

#endif /* QUICKNET_NETWORKNODE_H */
