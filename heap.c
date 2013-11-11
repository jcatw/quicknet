#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "networknode.h"
#include "heap.h"


heap_t *
make_heap() {
  heap_t *h = (heap_t*) malloc(sizeof(*h));
  if(!h) return 0;
  h->total_mass = 0;
  h->n_nodes = 0;
  //h->root = NULL;
  h->n_alloced = 64;
  h->items = (heap_item_t**) malloc(h->n_alloced * sizeof(heap_item_t*));
  return h;
}

heap_item_t *
make_heap_item(node_t *n, double (*compute_mass) (node_t*), double (*compute_priority) (heap_item_t*)) {
  heap_item_t *hi = (heap_item_t*) malloc(sizeof(*hi));
  if(!hi) return 0;
  hi->node = n;
  //hi->index = node->id;  // tight coupling
  hi->index = 0;
  hi->node_mass = (*compute_mass)(n);
  hi->subtree_mass = hi->node_mass;
  //hi->priority = (*compute_priority)(hi);
  return hi;
}

heap_item_t *
heap_left(heap_t *heap, heap_item_t *item) {
  uint64_t left_index = 2*item->index + 1;
  if (left_index < heap->n_nodes)
    return heap->items[left_index];
  else
    return NULL;
}

heap_item_t *
heap_right(heap_t *heap, heap_item_t *item) {
  uint64_t right_index = 2*item->index + 2;
  if (right_index < heap->n_nodes)
    return heap->items[right_index];
  else
    return NULL;
}

heap_item_t *
heap_parent(heap_t *heap, heap_item_t *item) {
  if(item->index == 0) // indices are unsigned
    return heap->items[0];
  return heap->items[(item->index-1) / 2];
}

void
heap_insert(heap_t *heap,
            node_t *node,
            double (*compute_mass) (node_t*),
            double (*compute_priority) (heap_item_t*)) {
  heap_item_t *item, *parent_item;
  double node_mass;
  uint64_t parent_index;
  
  item = make_heap_item(node, compute_mass, compute_priority);
  item->index = heap->n_nodes;
  
  // ensure that the heap can accommodate a new item
  if (heap->n_nodes == heap->n_alloced) {
    heap->items = realloc(heap->items, 2*heap->n_alloced * sizeof(heap_item_t*));
    if(!heap->items) {
      fprintf(stderr,"Could not reallocate heap (%llu -> %llu)!\n",heap->n_alloced,2*heap->n_alloced);
      return;
    }
    heap->n_alloced = 2*heap->n_alloced;
  }
  
  heap->items[item->index] = item;
  node_mass = item->node_mass;
  //parent_index = UINT64_MAX;
  //parent_item = item;
  //while (heap_parent(heap, parent_item)->index < parent_index) {
  while (heap_parent(heap, item) != item) {
    //parent_item = heap_parent(heap, parent_item);
    //parent_index = parent_item->index;
    //parent_item->subtree_mass += item->node_mass;
    item = heap_parent(heap, item);
    item->subtree_mass += node_mass;
  }

  heap->n_nodes++;
  heap->total_mass += node_mass;
}

void
heap_in_degree_insert(heap_t *heap, node_t *node) {
  heap_insert(heap, node, get_linear_in_degree, heap_item_get_node_mass);
}

void
heap_out_degree_insert(heap_t *heap, node_t *node) {
  heap_insert(heap, node, get_linear_out_degree, heap_item_get_node_mass);
}

void
heap_increase_priority(heap_t *heap,
                       heap_item_t* item,
                       double new_priority,
                       double (*compute_priority)(heap_item_t*),
                       void (*set_priority)(heap_item_t*,double)) {
  double new_mass;
  if (new_priority < compute_priority(item)) {
    fprintf(stderr,"New priority is smaller than current priority.\n");
    return;
  }

  // this is ugly and assumes node mass and priority are coupled, fix
  new_mass = new_priority - item->node_mass;
  
  set_priority(item,new_priority);
  
  
  item->node_mass = new_priority;
  item->subtree_mass += new_mass;
  
  while (item->index > 0 && compute_priority(heap_parent(heap, item)) < compute_priority(item)) {
    heap_parent(heap,item)->subtree_mass += new_mass;
    heap_exchange(heap, item, heap_parent(heap, item));
    item = heap_parent(heap, item);
  }

  while (item->index > 0) {
    heap_parent(heap,item)->subtree_mass += new_mass;
    item = heap_parent(heap, item);
  }
}

void
heap_exchange(heap_t *heap, heap_item_t *child, heap_item_t *parent) {
  heap_item_t *other_child;
  double other_child_subtree_mass;
  node_t *child_node, *parent_node;
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
}

node_t *
heap_sample_increment(heap_t *heap) {
  double uniform_sample;
  node_t *sampled_node;

  uniform_sample = rand() / (double)RAND_MAX;

  if(heap->n_nodes == 0)
    return NULL;
  
  sampled_node = heap_item_sample_increment(heap, heap->items[0], 0., uniform_sample);
  heap->total_mass += 1; //assumes linear
  return sampled_node;
}

node_t *
heap_item_sample_increment(heap_t *heap,
                           heap_item_t *item,
                           double observed_mass,
                           double uniform_sample) {
  node_t *sampled_node;

  if(heap_left(heap, item) != NULL) {
    if (uniform_sample < (observed_mass + heap_left(heap, item)->subtree_mass) / heap->total_mass) {
      sampled_node = heap_item_sample_increment(heap, heap_left(heap, item), observed_mass, uniform_sample);
      return sampled_node;
    }
    observed_mass += heap_left(heap, item)->subtree_mass;
  }

  observed_mass += item->node_mass;
  if (uniform_sample < observed_mass / heap->total_mass) {
    sampled_node = item->node;
    heap_increase_priority(heap, item, item->node_mass+1., heap_item_get_node_mass, heap_item_set_node_mass);
    return sampled_node;
  }

  if (heap_right(heap, item) != NULL) {
    sampled_node = heap_item_sample_increment(heap, heap_right(heap, item), observed_mass, uniform_sample);
    return sampled_node;
  }
  fprintf(stderr, "Failed to sample a heap node.\n");
  return NULL; //should not happen!
}

double
heap_item_get_node_mass(heap_item_t *item) {
  return item->node_mass;
}

void
heap_item_set_node_mass(heap_item_t *item, double node_mass) {
  item->node_mass = node_mass;
}

void
heap_free(heap_t *heap) {
  uint64_t i;
  for(i = 0; i<heap->n_nodes; i++) {
    free(heap->items[i]);
  }
  free(heap->items);
  free(heap);
}

//double
//heap_item_get_priority(heap_item_t *item) {
//  return item->priority;
//}
//
//void
//heap_item_set_priority(heap_item_t *item, double priority) {
//  item->priority = priority;
//}

//void
//heap_in_degree_insert(heap_t *heap, node_t* node) {
//  heap_insert(heap, node, get_linear_in_degree, heap_item_get_node_mass);
//}
//
//void
//heap_out_degree_insert(heap_t *heap, node_t* node) {
//  heap_insert(heap, node, get_linear_out_degree, heap_item_get_node_mass);
//}
