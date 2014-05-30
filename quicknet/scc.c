#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "networknode.h"
#include "model.h"
#include "scc.h"

static void scc_help(directed_node_t *node, nodestack_t *nstack, int64_t *index, int64_t *count_scc) {
  directed_node_list_t *other_l = NULL;
  directed_node_t *other = NULL;
  node->tarjindex = *index;
  node->tarjlowlink = *index;
  (*index)++;
  nstack = push_nodestack(nstack, node);

  other_l = node->adjacency_list;
  while(other_l) {
    other = other_l->node;
    if (other->tarjindex == -1) {
      //not visited yet
      scc_help(other, nstack, index, count_scc);
      if (other->tarjlowlink < node->tarjlowlink)
        node->tarjlowlink = other->tarjlowlink;
    }
    else if (nodestack_contains(nstack, other))
      if (other->tarjindex < node->tarjlowlink)
        node->tarjlowlink = other->tarjindex;
    other_l = other_l->next;

  }

  if(node->tarjlowlink == node->tarjindex) {
    //printf("tarjlowlink %d\n",node->tarjlowlink);
    //printf("tarjindex %d\n\n",node->tarjindex);
    do
      other = pop_nodestack(nstack);
    while (other != node);
    (*count_scc)++;
  }
}

//use tarjan's algorithm to discover whether a directed graph is fully
//connected (http://en.wikipedia.org/wiki/Tarjan's_strongly_connected_components_algorithm)
int is_strongly_connected(directed_model_t *model) {
  int i;
  int64_t index = 0;
  int64_t count_scc = 0;
  nodestack_t *nstack = NULL;
  for (i=0; i<model->n_nodes; i++) {
    //printf("%d\n",i);
    if (model->nodes[i]->tarjindex == -1) {
      scc_help(model->nodes[i], nstack, &index, &count_scc);
    }
    //printf("%d\n",i);
  }
  return count_scc;
}

nodestack_t *make_nodestack(directed_node_t *node) {
  nodestack_t *nstack = (nodestack_t*) malloc(sizeof(nodestack_t));
  nstack->node = node;
  return nstack;
}

directed_node_t *pop_nodestack(nodestack_t *nstack) {
  directed_node_t *node = NULL;
  nodestack_t *top;
  if (nstack == NULL)
    return NULL;
  node = nstack->node;
  top = nstack;
  nstack = nstack->next;
  free(top);
  node->on_stack = 0;
  return node;
}

nodestack_t *push_nodestack(nodestack_t *nstack, directed_node_t *node) {
  nodestack_t *new = NULL;
  new = make_nodestack(node);
  new->next = nstack;
  nstack = new;
  node->on_stack = 1;
  return nstack;
}

void free_nodestack(nodestack_t *nstack) {
  nodestack_t *next;

  while(nstack) {
    next = nstack->next;
    free(nstack);
    nstack = next;
  }
}

char nodestack_contains(nodestack_t *nstack, directed_node_t *query_node) {
  return query_node->on_stack;
  //while(nstack) {
  //  if (nstack->node == query_node)
  //    return 1;
  //  nstack = nstack->next;
  //}
  //return 0;
}
