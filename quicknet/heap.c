#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#define HASH_BUCKETS 10000

#include "networknode.h"
#include "heap.h"
#include "hash.h"

heap_t *make_heap() {
  heap_t *h = (heap_t*) malloc(sizeof(*h));
  if(!h) {
    fprintf(stderr, "Out of Memory.\n");
    exit(1);
  }
  h->total_mass = 0;
  h->n_nodes = 0;
  h->hash = make_hash(HASH_BUCKETS);
  h->n_alloced = 64;
  h->items = (heap_item_t**) malloc(h->n_alloced * sizeof(heap_item_t*));
  return h;
}

heap_item_t * make_heap_item(directed_node_t *n, double (*compute_mass) (directed_node_t*)) {
  heap_item_t *hi = (heap_item_t*) malloc(sizeof(*hi));
  if(!hi) {
    fprintf(stderr, "Out of memory.\n");
    exit(1);
  }
  hi->node = n;
  hi->index = 0;
  hi->node_mass = (*compute_mass)(n);
  hi->subtree_mass = hi->node_mass;
  return hi;
}

heap_item_t *heap_left(heap_t *heap, heap_item_t *item) {
  uint64_t left_index = 2*item->index + 1;
  if (left_index < heap->n_nodes)
    return heap->items[left_index];
  else
    return NULL;
}

heap_item_t *heap_right(heap_t *heap, heap_item_t *item) {
  uint64_t right_index = 2*item->index + 2;
  if (right_index < heap->n_nodes)
    return heap->items[right_index];
  else
    return NULL;
}

heap_item_t *heap_parent(heap_t *heap, heap_item_t *item) {
  if(item->index == 0) // indices are unsigned
    return heap->items[0];
  return heap->items[(item->index-1) / 2];
}

void
heap_insert(heap_t *heap,
            directed_node_t *node,
            double (*compute_mass) (directed_node_t*)) {
  heap_item_t *item;
  //heap_item_t *parent_item;
  double node_mass;
  //uint64_t parent_index;
  
  item = make_heap_item(node, compute_mass);
  item->index = heap->n_nodes;

  hash_insert(heap->hash, node, item);
  
  // ensure that the heap can accommodate a new item
  if (heap->n_nodes == heap->n_alloced) {
    heap->items = realloc(heap->items, 2*heap->n_alloced * sizeof(heap_item_t*));
    if(!heap->items) {
      fprintf(stderr,"Could not reallocate heap (%llu -> %llu)!\n",heap->n_alloced,2*heap->n_alloced);
      exit(1);
    }
    heap->n_alloced = 2*heap->n_alloced;
  }
  
  heap->items[item->index] = item;
  node_mass = item->node_mass;
  while (heap_parent(heap, item) != item) {
    item = heap_parent(heap, item);
    item->subtree_mass += node_mass;
  }

  heap->n_nodes++;
  heap->total_mass += node_mass;
}

void heap_increase_mass(heap_t *heap,
                        heap_item_t* item,
                        double new_mass) {
  // assumes that priority and node mass are one and the same
  double introduced_mass;
  if (new_mass < item->node_mass) {
    fprintf(stderr,"New priority %.6f is smaller than current priority %.6f.\n",new_mass,item->node_mass);
    return;
  }

  introduced_mass = new_mass - item->node_mass;

  item->node_mass = new_mass;
  item->subtree_mass += introduced_mass;
  
  while (item->index > 0 && (heap_parent(heap, item)->node_mass < item->node_mass)) {
    heap_parent(heap,item)->subtree_mass += introduced_mass;
    heap_exchange(heap, item, heap_parent(heap, item));
    item = heap_parent(heap, item);
  }

  while (item->index > 0) {
    heap_parent(heap,item)->subtree_mass += introduced_mass;
    item = heap_parent(heap, item);
  }
}

void heap_exchange(heap_t *heap, heap_item_t *child, heap_item_t *parent) {
  heap_item_t *other_child;
  double other_child_subtree_mass;
  directed_node_t *child_node, *parent_node;
  double child_node_mass, parent_node_mass;
  //double child_subtree_mass, parent_subtree_mass;
  double parent_subtree_mass;

  if(heap_left(heap, parent) == child)
    other_child = heap_right(heap,parent);
  else if (heap_right(heap, parent) == child)
    other_child = heap_left(heap,parent);
  else {
    fprintf(stderr,"No parent-child relationship between heap items %llu and %llu\n",child->index,parent->index);
    return;
  }

  if(other_child == NULL) {
    other_child_subtree_mass = 0.;
  }
  else {
    other_child_subtree_mass = other_child->subtree_mass;
  }

  child_node = child->node;
  parent_node = parent->node;
  child_node_mass = child->node_mass;
  parent_node_mass = parent->node_mass;
  //child_subtree_mass = child->subtree_mass;
  parent_subtree_mass = parent->subtree_mass;
  
  parent->node = child_node;
  child->node = parent_node;
  parent->node_mass = child_node_mass;
  child->node_mass = parent_node_mass;
  //child->subtree_mass = parent_subtree_mass - child_node_mass + parent_node_mass;
  child->subtree_mass = parent_subtree_mass - child_node_mass - other_child_subtree_mass;

  hash_get(heap->hash, parent->node)->item = parent;
  hash_get(heap->hash, child->node)->item = child;
}

directed_node_t *heap_sample_increment(heap_t *heap,
                                       double (*compute_increased_mass)(directed_node_t *node)) {
  double uniform_sample;
  directed_node_t *sampled_node;

  uniform_sample = rand() / (double)RAND_MAX;

  if(heap->n_nodes == 0)
    return NULL;
  
  sampled_node = heap_item_sample_increment(heap,
                                            heap->items[0],
                                            0.,
                                            uniform_sample,
                                            compute_increased_mass);

  // no longer do this - increment total mass in heap_item_sample_increment
  //heap->total_mass += 1; //assumes linear
  
  return sampled_node;
}

directed_node_t *heap_item_sample_increment(heap_t *heap,
                                            heap_item_t *item,
                                            double observed_mass,
                                            double uniform_sample,
                                            double (*compute_increased_mass)(directed_node_t *node)) {
  directed_node_t *sampled_node;
  double old_mass;
  
  if(heap_left(heap, item) != NULL) {
    if (uniform_sample < (observed_mass + heap_left(heap, item)->subtree_mass) / heap->total_mass) {
      sampled_node = heap_item_sample_increment(heap, heap_left(heap, item), observed_mass, uniform_sample,compute_increased_mass);
      return sampled_node;
    }
    observed_mass += heap_left(heap, item)->subtree_mass;
  }

  observed_mass += item->node_mass;
  if (uniform_sample < observed_mass / heap->total_mass) {
    old_mass = item->node_mass;
    sampled_node = item->node;
    heap_increase_mass(heap,
                       item,
                       compute_increased_mass(sampled_node));
    heap->total_mass += item->node_mass - old_mass;
    return sampled_node;
  }

  if (heap_right(heap, item) != NULL) {
    sampled_node = heap_item_sample_increment(heap, heap_right(heap, item), observed_mass, uniform_sample, compute_increased_mass);
    return sampled_node;
  }
  fprintf(stderr, "Failed to sample a heap node.\n");
  exit(1);
  //return NULL; //should not happen!
}

void heap_free(heap_t *heap) {
  uint64_t i;
  for(i = 0; i<heap->n_nodes; i++) {
    free(heap->items[i]);
  }
  free(heap->items);
  free(heap);
}
