#ifndef QUICKNET_SCC_H
#define QUICKNET_SCC_H

#include "types.h"

struct nodestack {
  directed_node_t *node;
  nodestack_t *next;
};

nodestack_t *make_nodestack(directed_node_t *node);
directed_node_t *pop_nodestack(nodestack_t *nstack);
nodestack_t *push_nodestack(nodestack_t *nstack, directed_node_t *node);
void free_nodestack(nodestack_t *nstack);
char nodestack_contains(nodestack_t *nstack, directed_node_t *query_node);
int is_strongly_connected(directed_model_t *model);
#endif /* QUICKNET_SCC_H */
