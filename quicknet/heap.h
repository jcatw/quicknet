#ifndef QUICKNET_HEAP_H
#define QUICKNET_HEAP_H

#include "types.h"

struct heap {
  double total_mass;
  uint64_t n_nodes;
  hash_t *hash;
  heap_item_t **items;
  uint64_t n_alloced;
};

struct heap_item {
  directed_node_t *node;
  uint64_t index;
  double node_mass;
  double subtree_mass;
};

heap_t *make_heap();
heap_item_t *make_heap_item(directed_node_t *n, double (*compute_mass) (directed_node_t*));
heap_item_t *heap_left  (heap_t *heap, heap_item_t *item);
heap_item_t *heap_right (heap_t *heap, heap_item_t *item);
heap_item_t *heap_parent(heap_t *heap, heap_item_t *item);
void heap_insert(heap_t *heap, directed_node_t* node, double (*compute_mass) (directed_node_t*));
void heap_increase_mass(heap_t *heap, heap_item_t* item, double new_mass);
void heap_exchange(heap_t *heap, heap_item_t *child, heap_item_t *parent);
directed_node_t *heap_sample_increment(heap_t *heap,
                              double (*compute_increased_mass)(directed_node_t *node));
directed_node_t *heap_item_sample_increment(heap_t *heap,
                                   heap_item_t *item,
                                   double observed_mass,
                                   double uniform_sample,
                                   double (*compute_increased_mass)(directed_node_t *node));
void heap_free(heap_t *heap);


#endif /* QUICKNET_HEAP_H */
