#ifndef QUICKNET_HASH_H
#define QUICKNET_HASH_H

#include "types.h"

struct hash {
  int n_buckets;
  hash_item_t **buckets;
};

struct hash_item {
  directed_node_t *node;
  heap_item_t *item;
  hash_item_t *next;
};

hash_t *make_hash(int n_buckets);
hash_item_t *make_hash_item(directed_node_t *node, heap_item_t *item);
int hash_addr(hash_t *table, directed_node_t *node);
void hash_insert(hash_t *table, directed_node_t *node, heap_item_t *item);
hash_item_t *hash_get(hash_t *table, directed_node_t *node);

#endif /* QUICKNET_HEAP_H */
