#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "networknode.h"
#include "bstreap.h"


struct bstreap {
  double total_mass;
  bstreap_item_t *root;
};

struct bstreap_item {
  node_t *node;
  bstreap_item_t *left;
  bstreap_item_t *right;
  double node_mass;
  double subtree_mass;
  uint64_t priority;
};

// general functions
bstreap_t *
make_bstreap() {
  bstreap_t *b = (bstreap_t*) malloc(sizeof(*b));
  if (!b) return 0;
  b->total_mass = 0.;
  b->root = NULL;
  return b;
}

bstreap_item_t *
make_bstreap_item(node_t *n, double (*compute_mass) (node_t*), uint64_t (*compute_priority) (node_t*)) {
  bstreap_item_t *bi = (bstreap_item_t*) malloc(sizeof(*bi));
  if(!bi) return 0;
  bi->node = n;
  bi->left = NULL;
  bi->right = NULL;
  bi->node_mass = (*compute_mass)(n);
  bi->subtree_mass = bi->node_mass;
  bi->priority = (*compute_priority)(n);
  return bi;
}

//bstreap_item_t *
void
bstreap_insert(bstreap_t *bstreap,
               node_t* node,
               double (*compute_mass) (node_t*),
               uint64_t (*compute_priority) (node_t*),
               double (*compute_key)(bstreap_item_t*)) {
  bstreap_item_t *bstreap_item;
  //bstreap_item_t *parent;
  bstreap_item = make_bstreap_item(node, compute_mass, compute_priority);

  if(bstreap->root == NULL) {
    bstreap->root = bstreap_item;
    //parent = NULL;
  }
  else {
    //parent = bstreap_item_insert(bstreap, bstreap_item, bstreap->root, NULL, compute_key);
    bstreap_item_insert(bstreap, bstreap_item, bstreap->root, NULL, compute_key);
  }
  bstreap->total_mass += bstreap_item->node_mass;
  //return parent;
}

//bstreap_item_t *
void
bstreap_item_insert(bstreap_t *bstreap,
                      bstreap_item_t *item,
                      bstreap_item_t *existing_item,
                      bstreap_item_t *parent,
                      double (*compute_key)(bstreap_item_t*)) {
existing_item->subtree_mass += item->node_mass;

  // if the sort key of the inserted node is smaller than the existing node...
  if ((*compute_key)(item) <= (*compute_key)(existing_item)) {
    // ...and the existing node has nothing to the left
    if (existing_item->left == NULL) {
      // then add the new node on the left
      existing_item->left = item;
    }
    // ...and the existing node has something to the left
    else {
      // then add the new node to the left recursively
      bstreap_item_insert(bstreap,
                          item,
                          existing_item->left,
                          existing_item,
                          compute_key);      
    }
    // finally, ensure that the heap property is maintained
    if (existing_item->left->priority > existing_item->priority)
      bstreap_rotate_right(bstreap, existing_item, parent);
    return;
  }
  // if the sort key of the inserted node is larger than the existing node...
  else {
    // ...and the existing node has nothing to the right
    if(existing_item->right == NULL) {
      // then add the new node to the right
      existing_item->right = item;
    }
    // ...and the existing node has something to the right
    else {
      // then add the new node to the right recursively
      bstreap_item_insert(bstreap,
                          item,
                          existing_item->right,
                          existing_item,
                          compute_key);
      
    }
    // finally, ensure that the heap property is maintained
    if (existing_item->right->priority > existing_item->priority)
      bstreap_rotate_left(bstreap, existing_item, parent);
    return;
  }
}

void bstreap_rotate_left(bstreap_t *bstreap, bstreap_item_t *item, bstreap_item_t *parent) {
  bstreap_item_t *y;
  double full_subtree_mass;

  full_subtree_mass = item->subtree_mass;

  y = item->right;
  y->subtree_mass = full_subtree_mass;

  item->right = y->left;  // assumes y is non-NULL
  item->subtree_mass -= y->node_mass;
  if(y->right != NULL)
    item->subtree_mass -= y->right->node_mass;

  y->left = item;
    
  if(parent == NULL)
    bstreap->root = y;
  else if(parent->left == item)
    parent->left = y;
  else if(parent->right == item)
    parent->right = y;
  else
    return; //should not happen!
}


void bstreap_rotate_right(bstreap_t *bstreap, bstreap_item_t *item, bstreap_item_t *parent) {
  bstreap_item_t *y;
  double full_subtree_mass;

  full_subtree_mass = item->subtree_mass;

  y = item->left;
  y->subtree_mass = full_subtree_mass;

  item->left = y->right; // assumes y is non-NULL
  item->subtree_mass -= y->node_mass;
  if(y->left != NULL)
    item->subtree_mass -= y->left->node_mass;

  y->left = item;

  if(parent == NULL)
    bstreap->root = y;
  else if(parent->left == item)
    parent->left = y;
  else if(parent->right == item)
    parent->right = y;
  else
    return; //should not happen!
}

node_t *
bstreap_sample(bstreap_t* bstreap) {
  bstreap_item_t *sampled_item;
  node_t *sampled_node;
  double uniform_sample;

  uniform_sample = rand() / (double)RAND_MAX;

  if(bstreap->root == NULL)
    return NULL;
  
  sampled_item = bstreap_item_sample(bstreap, bstreap->root, NULL, 0., uniform_sample);
  sampled_node = sampled_item->node;
  bstreap->total_mass -= sampled_item->node_mass;

  free(sampled_item);

  return sampled_node;
}

bstreap_item_t *
bstreap_item_sample(bstreap_t *bstreap,
                    bstreap_item_t *item,
                    bstreap_item_t *parent,
                    double observed_mass,
                    double uniform_sample) {
  bstreap_item_t *sampled_item;
  if (item->left != NULL) {
    if (uniform_sample < (observed_mass + item->left->subtree_mass) / bstreap->total_mass) {
      sampled_item = bstreap_item_sample(bstreap, item->left, item, observed_mass, uniform_sample);
      item->subtree_mass -= sampled_item->node_mass;
      return sampled_item;
    }
    observed_mass += item->left->subtree_mass;
  }

  observed_mass += item->node_mass;
  if (uniform_sample < observed_mass / bstreap->total_mass) {
    bstreap_item_splice(bstreap, item, parent);
    return item;
  }

  if (item->right != NULL) {
    sampled_item = bstreap_item_sample(bstreap, item->right, item, observed_mass, uniform_sample);
    item->subtree_mass -= sampled_item->node_mass;
    return sampled_item;
  }
  fprintf(stderr,"Failed to sample a node.\n");
  return NULL; //should not happen!
}

// remove a node from a bstreap
// do not free wrapper
void bstreap_item_splice(bstreap_t *bstreap, bstreap_item_t *item, bstreap_item_t *parent) {
  while(!(item->left == NULL && item->right == NULL)) {
    if(item->left == NULL) {
      bstreap_rotate_left(bstreap, item, parent);
      parent = item;
      item = item->left;
    }
    else if (item->right == NULL) {
      bstreap_rotate_right(bstreap, item, parent);
      parent = item;
      item = item->right;
    }
    else if (item->left->priority > item->right->priority) {
      bstreap_rotate_right(bstreap, item, parent);
      parent = item;
      item = item->right;
    }
    else {
      bstreap_rotate_left(bstreap, item, parent);
      parent = item;
      item = item->left;
    }
  }
  if (bstreap->root == item)
    bstreap->root = NULL;
  else if (parent->left == item)
    parent->left = NULL;
  else if (parent->right == item)
    parent->right = NULL;
}

// abstraction providers

double
get_node_mass (bstreap_item_t *item) {
  return item->node_mass;
}

uint64_t
random_priority () {
  return rand();
}

//curried functions

bstreap_item_t *
make_in_degree_bstreap_item_xnu(node_t *n) {
  return make_bstreap_item(n, get_in_degree, random_priority);
}

bstreap_item_t *
make_out_degree_bstreap_item_xnu(node_t *n) {
  return make_bstreap_item(n, get_out_degree, random_priority);
}

void
bstreap_in_degree_insert_lnu(bstreap_t *bstreap, node_t* node) {
  bstreap_insert(bstreap, node, get_in_degree, random_priority, get_node_mass);
}

void
bstreap_out_degree_insert_lnu(bstreap_t *bstreap, node_t* node) {
  bstreap_insert(bstreap, node, get_out_degree, random_priority, get_node_mass);
}

void bstreap_item_insert_xnz(bstreap_t *bstreap,
                             bstreap_item_t *item,
                             bstreap_item_t *existing_item,
                             bstreap_item_t *parent) {
  bstreap_item_insert(bstreap, item, existing_item, parent, get_node_mass);
}
