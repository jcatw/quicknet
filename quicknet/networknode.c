#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "networknode.h"

directed_node_t *make_directed_node(uint64_t id, double lambda, double mu) {
  directed_node_t *n = (directed_node_t*) malloc(sizeof(*n));
  if (!n) {
    fprintf(stderr, "Out of memory.\n");
    exit(1);
  }
  n->id = id;
  n->in_degree = 0;
  n->out_degree = 0;
  n->lambda = lambda;
  n->mu = mu;
  n->adjacency_list = NULL;
  return n;
}

directed_node_t *clone_directed_node(directed_node_t *node) {
  directed_node_t *cloned_node = (directed_node_t*) malloc(sizeof(*cloned_node));
  if (!cloned_node) {
    fprintf(stderr, "Out of memory.\n");
    exit(1);
  }
  cloned_node->id = node->id;
  cloned_node->in_degree = node->in_degree;
  cloned_node->out_degree = node->out_degree;
  cloned_node->lambda = node->lambda;
  cloned_node->mu = node->mu;
  cloned_node->adjacency_list = node->adjacency_list;

  return cloned_node;
}
  

directed_node_list_t *make_directed_node_list(directed_node_t *n) {
  directed_node_list_t *nl = (directed_node_list_t*) malloc(sizeof(*nl));
  if (!nl) {
    fprintf(stderr, "Out of memory.\n");
    exit(1);
  }
  nl->node = n;
  nl->next = NULL;
  return nl;
}

void add_directed_edge(directed_node_t *n_from, directed_node_t *n_to) {
  directed_node_list_t *wrapped_n_to = make_directed_node_list(n_to);
  directed_node_list_t *old_front = n_from->adjacency_list;
  n_from->adjacency_list = wrapped_n_to;
  wrapped_n_to->next = old_front;
  n_from->out_degree++;
  n_to->in_degree++;
}

void directed_node_free(directed_node_t *node, char free_adjacency_list) {
  if(free_adjacency_list && node->adjacency_list != NULL)
    directed_node_list_free(node->adjacency_list);
  free(node);
}

void directed_node_list_free(directed_node_list_t *node_list) {
  directed_node_list_t *tmp;
  while(node_list != NULL) {
    tmp = node_list->next;
    free(node_list);
    node_list = tmp;
  }
}
