#include <stdlib.h>
#include <stdint.h>

#include "networknode.h"

node_t *
make_node(uint64_t id, double lambda, double mu) {
  node_t *n = (node_t*) malloc(sizeof(*n));
  if (!n) return 0;
  n->id = id;
  n->in_degree = 0;
  n->out_degree = 0;
  n->lambda = lambda;
  n->mu = mu;
  n->adjacency_list = NULL;
  return n;
}

//node_list_t *
//make_node_list() {
//  node_list_t *nl = (node_list_t*) malloc(sizeof(*nl));
//  if (!nl) return 0;
//  nl->node = NULL;
//  nl->next = NULL;
//  return nl;
//}

node_list_t *
make_node_list(node_t *n) {
  node_list_t *nl = (node_list_t*) malloc(sizeof(*nl));
  if (!nl) return 0;
  nl->node = n;
  nl->next = NULL;
  return nl;
}

void
add_edge(node_t *n_from, node_t *n_to) {
  node_list_t *wrapped_n_to = make_node_list(n_to);
  node_list_t *old_front = n_from->adjacency_list;
  n_from->adjacency_list = wrapped_n_to;
  wrapped_n_to->next = old_front;
  n_from->out_degree++;
  n_to->in_degree++;
}

double get_in_degree(node_t *node) {
  return (double) node->in_degree;
}

double get_linear_in_degree(node_t *node) {
  return (double) node->in_degree + node->lambda;
}

double get_out_degree(node_t *node) {
  return (double) node->out_degree;
}

double get_linear_out_degree(node_t *node) {
  return (double) node->out_degree + node->mu;
}

void node_free(node_t *node) {
  if(node->adjacency_list != NULL)
    node_list_free(node->adjacency_list);
  free(node);
}

void node_list_free(node_list_t *node_list) {
  if(node_list->next != NULL)
    node_list_free(node_list->next);
  free(node_list);
}
