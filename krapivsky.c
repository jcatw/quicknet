#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "networknode.h"
#include "heap.h"
#include "bstreap.h"
#include "krapivsky.h"
#include "quickmath.h"

// This architecture lead to a lot of repitition.  This macro defines
// that is needed for a new preference function.
#define MAKE_KRAPIV_PREF_FUNCTIONS(name) \
  void \
  make_heap_item_seed_ ## name(krapivsky_model_t *km) { \
  node_t *node0, *node1, *node2; \
  node0 = make_node(0,km->lambda,km->mu); \
  node1 = make_node(1,km->lambda,km->mu); \
  node2 = make_node(2,km->lambda,km->mu); \
   \
  add_edge(node0,node1); \
  add_edge(node0,node2); \
  add_edge(node1,node2); \
 \
  heap_in_degree_ ## name ## _insert(km->in_degree_set.heap,node0); \
  heap_in_degree_ ## name ## _insert(km->in_degree_set.heap,node1); \
  heap_in_degree_ ## name ## _insert(km->in_degree_set.heap,node2); \
  heap_out_degree_ ## name ## _insert(km->out_degree_set.heap,node0); \
  heap_out_degree_ ## name ## _insert(km->out_degree_set.heap,node1); \
  heap_out_degree_ ## name ## _insert(km->out_degree_set.heap,node2); \
 \
  km->n_nodes = 3; \
  km->nodes[0] = node0; \
  km->nodes[1] = node1; \
  km->nodes[2] = node2; \
  }                     \
  node_t *\
  krapivsky_heap_ ## name ## _in_degree_sampler(krapivsky_model_t *km) {\
    return heap_sample_increment_ ## name ## _in_degree(km->in_degree_set.heap);\
  }\
  node_t *                                                              \
  krapivsky_heap_ ## name ## _out_degree_sampler(krapivsky_model_t *km) {  \
    return heap_sample_increment_ ## name ## _out_degree(km->out_degree_set.heap); \
  }\
  void  \
  krapivsky_heap_in_degree_ ## name ## _indexer(krapivsky_model_t *km, node_t *node) { \
    heap_in_degree_ ## name ## _insert(km->in_degree_set.heap, node);      \
  }                                                                     \
  void                                                                  \
  krapivsky_heap_out_degree_ ## name ## _indexer(krapivsky_model_t *km, node_t *node) { \
    heap_out_degree_ ## name ## _insert(km->out_degree_set.heap, node);    \
  }                                                                     \
  void \
  krapivsky_heap_ ## name ## _node_adder(krapivsky_model_t *km, node_t *new_node) { \
    heap_in_degree_ ## name ## _insert(km->in_degree_set.heap, new_node);  \
    heap_out_degree_ ## name ## _insert(km->out_degree_set.heap, new_node); \
  }                                                                     \
  krapivsky_model_t \
  *krapivsky_heap_simulate_ ## name(krapivsky_input_t *input) {        \
    krapivsky_model_t *km;                                              \
    krapivsky_input_heap(input);                                        \
    krapivsky_input_ ## name ## _heap(input);                              \
    km = krapivsky_simulate(input);                                     \
    return km;                                                          \
  }                                                                     \
  void                                                                  \
  krapivsky_input_ ## name ## _heap(krapivsky_input_t *input) {         \
  input->seed_maker = make_heap_item_seed_ ## name ; \
  input->node_adder = krapivsky_heap_ ## name ## _node_adder; \
  input->new_node_in_degree_indexer  = krapivsky_heap_in_degree_ ## name ## _indexer; \
  input->new_node_out_degree_indexer = krapivsky_heap_out_degree_ ## name ## _indexer; \
  input->in_degree_sampler = krapivsky_heap_ ## name ## _in_degree_sampler; \
  input->out_degree_sampler = krapivsky_heap_ ## name ## _out_degree_sampler; \
  } 

krapivsky_model_t *
make_krapivsky_model(double p,
                     double lambda,
                     double mu,
                     uint64_t target_n_nodes,
                     void  (*set_maker)(krapivsky_model_t*),
                     void  (*seed_maker)(krapivsky_model_t*),
                     double (*fitness_function)(double fitness),
                     uint8_t index_type) {
  krapivsky_model_t *km = (krapivsky_model_t*) malloc(sizeof(*km));
  if (!km) return 0;
  (*set_maker)(km);
  km->index_type = index_type;
  km->p = p;
  km->lambda = (*fitness_function)(lambda);
  km->mu = (*fitness_function)(mu);
  km->nodes = (node_t**) malloc(target_n_nodes * sizeof(node_t**));
  (*seed_maker)(km);  
  return km;
}

void
make_bstreap_item_seed_lnu(krapivsky_model_t *km) {
  node_t *node0, *node1, *node2;
  node0 = make_node(0,km->lambda,km->mu);
  node1 = make_node(1,km->lambda,km->mu);
  node2 = make_node(2,km->lambda,km->mu);
  
  // 0->1, 0->2, 1->2
  add_edge(node0,node1);
  add_edge(node0,node2);
  add_edge(node1,node2);
  bstreap_in_degree_insert_lnu(km->in_degree_set.bstreap,node0);
  bstreap_in_degree_insert_lnu(km->in_degree_set.bstreap,node1);
  bstreap_in_degree_insert_lnu(km->in_degree_set.bstreap,node2);
  bstreap_out_degree_insert_lnu(km->out_degree_set.bstreap,node0);
  bstreap_out_degree_insert_lnu(km->out_degree_set.bstreap,node1);
  bstreap_out_degree_insert_lnu(km->out_degree_set.bstreap,node2);

  km->n_nodes = 3;
  km->nodes[0] = node0;
  km->nodes[1] = node1;
  km->nodes[2] = node2;
}

void
make_bstreap_item_seed_lnn(krapivsky_model_t *km) {
  node_t *node0, *node1, *node2;
  node0 = make_node(0,km->lambda,km->mu);
  node1 = make_node(1,km->lambda,km->mu);
  node2 = make_node(2,km->lambda,km->mu);
  
  // 0->1, 0->2, 1->2
  add_edge(node0,node1);
  add_edge(node0,node2);
  add_edge(node1,node2);
  bstreap_in_degree_insert_lnn(km->in_degree_set.bstreap,node0);
  bstreap_in_degree_insert_lnn(km->in_degree_set.bstreap,node1);
  bstreap_in_degree_insert_lnn(km->in_degree_set.bstreap,node2);
  bstreap_out_degree_insert_lnn(km->out_degree_set.bstreap,node0);
  bstreap_out_degree_insert_lnn(km->out_degree_set.bstreap,node1);
  bstreap_out_degree_insert_lnn(km->out_degree_set.bstreap,node2);

  km->n_nodes = 3;
  km->nodes[0] = node0;
  km->nodes[1] = node1;
  km->nodes[2] = node2;
}

void
make_bstreap_item_seed_lsu(krapivsky_model_t *km) {
  node_t *node0, *node1, *node2;
  node0 = make_node(0,km->lambda,km->mu);
  node1 = make_node(1,km->lambda,km->mu);
  node2 = make_node(2,km->lambda,km->mu);
  
  // 0->1, 0->2, 1->2
  add_edge(node0,node1);
  add_edge(node0,node2);
  add_edge(node1,node2);
  bstreap_in_degree_insert_lsu(km->in_degree_set.bstreap,node0);
  bstreap_in_degree_insert_lsu(km->in_degree_set.bstreap,node1);
  bstreap_in_degree_insert_lsu(km->in_degree_set.bstreap,node2);
  bstreap_out_degree_insert_lsu(km->out_degree_set.bstreap,node0);
  bstreap_out_degree_insert_lsu(km->out_degree_set.bstreap,node1);
  bstreap_out_degree_insert_lsu(km->out_degree_set.bstreap,node2);

  km->n_nodes = 3;
  km->nodes[0] = node0;
  km->nodes[1] = node1;
  km->nodes[2] = node2;
}

void
make_bstreap_item_seed_lsn(krapivsky_model_t *km) {
  node_t *node0, *node1, *node2;
  node0 = make_node(0,km->lambda,km->mu);
  node1 = make_node(1,km->lambda,km->mu);
  node2 = make_node(2,km->lambda,km->mu);
  
  // 0->1, 0->2, 1->2
  add_edge(node0,node1);
  add_edge(node0,node2);
  add_edge(node1,node2);
  bstreap_in_degree_insert_lsn(km->in_degree_set.bstreap,node0);
  bstreap_in_degree_insert_lsn(km->in_degree_set.bstreap,node1);
  bstreap_in_degree_insert_lsn(km->in_degree_set.bstreap,node2);
  bstreap_out_degree_insert_lsn(km->out_degree_set.bstreap,node0);
  bstreap_out_degree_insert_lsn(km->out_degree_set.bstreap,node1);
  bstreap_out_degree_insert_lsn(km->out_degree_set.bstreap,node2);

  km->n_nodes = 3;
  km->nodes[0] = node0;
  km->nodes[1] = node1;
  km->nodes[2] = node2;
}

void
make_heap_item_seed_linear(krapivsky_model_t *km) {
  node_t *node0, *node1, *node2;
  node0 = make_node(0,km->lambda,km->mu);
  node1 = make_node(1,km->lambda,km->mu);
  node2 = make_node(2,km->lambda,km->mu);
  
  // 0->1, 0->2, 1->2
  add_edge(node0,node1);
  add_edge(node0,node2);
  add_edge(node1,node2);

  heap_in_degree_linear_insert(km->in_degree_set.heap,node0);
  heap_in_degree_linear_insert(km->in_degree_set.heap,node1);
  heap_in_degree_linear_insert(km->in_degree_set.heap,node2);
  heap_out_degree_linear_insert(km->out_degree_set.heap,node0);
  heap_out_degree_linear_insert(km->out_degree_set.heap,node1);
  heap_out_degree_linear_insert(km->out_degree_set.heap,node2);

  km->n_nodes = 3;
  km->nodes[0] = node0;
  km->nodes[1] = node1;
  km->nodes[2] = node2;
}

void
make_heap_item_seed_quadratic(krapivsky_model_t *km) {
  node_t *node0, *node1, *node2;
  node0 = make_node(0,km->lambda,km->mu);
  node1 = make_node(1,km->lambda,km->mu);
  node2 = make_node(2,km->lambda,km->mu);
  
  // 0->1, 0->2, 1->2
  add_edge(node0,node1);
  add_edge(node0,node2);
  add_edge(node1,node2);

  heap_in_degree_quadratic_insert(km->in_degree_set.heap,node0);
  heap_in_degree_quadratic_insert(km->in_degree_set.heap,node1);
  heap_in_degree_quadratic_insert(km->in_degree_set.heap,node2);
  heap_out_degree_quadratic_insert(km->out_degree_set.heap,node0);
  heap_out_degree_quadratic_insert(km->out_degree_set.heap,node1);
  heap_out_degree_quadratic_insert(km->out_degree_set.heap,node2);

  km->n_nodes = 3;
  km->nodes[0] = node0;
  km->nodes[1] = node1;
  km->nodes[2] = node2;
}

int8_t
krapivsky_done(krapivsky_model_t *km, uint64_t target_n_nodes) {
  return km->n_nodes >= target_n_nodes;
}

void
krapivsky_next(krapivsky_model_t *km,
               void (*node_adder) (krapivsky_model_t *km, node_t *node),
               node_t* (*in_degree_sampler) (krapivsky_model_t *km),
               node_t* (*out_degree_sampler) (krapivsky_model_t *km),
               void (*new_node_in_degree_indexer) (krapivsky_model_t *km, node_t *node),
               void (*new_node_out_degree_indexer) (krapivsky_model_t *km, node_t *node),
               void (*existing_node_in_degree_indexer) (krapivsky_model_t *km, node_t *node),
               void (*existing_node_out_degree_indexer) (krapivsky_model_t *km, node_t *node)) {
  double uniform_sample;
  node_t *new_node, *in_degree_sampled_node, *out_degree_sampled_node;

  uniform_sample = rand() / (double)RAND_MAX;

  // node addition step
  if(uniform_sample < km->p) {
    // abstraction: in degree sampler
    in_degree_sampled_node = (*in_degree_sampler)(km);

    new_node = make_node(km->n_nodes,km->lambda,km->mu);
    km->nodes[km->n_nodes] = new_node;
    km->n_nodes++;

    // no further abstraction
    add_edge(new_node,in_degree_sampled_node);

    // abstraction: node adder
    //(*node_adder)(km, new_node);

    // abstraction: in, out degree indexers
    (*new_node_out_degree_indexer)(km, new_node);
    (*new_node_in_degree_indexer)(km, new_node);

    (*existing_node_in_degree_indexer)(km, in_degree_sampled_node);
  }
  else { //edge step
    // abstraction: in, out degree samplers
    out_degree_sampled_node = (*out_degree_sampler)(km);
    in_degree_sampled_node = (*in_degree_sampler)(km);

    // no further abstraction
    add_edge(out_degree_sampled_node, in_degree_sampled_node);

    // abstraction: in, out degree indexers
    (*existing_node_out_degree_indexer)(km, out_degree_sampled_node);
    (*existing_node_in_degree_indexer)(km, in_degree_sampled_node);
  }
}

krapivsky_model_t*
krapivsky_simulate(krapivsky_input_t *input) {
  krapivsky_model_t *km = make_krapivsky_model(input->p,
                                               input->lambda,
                                               input->mu,
                                               input->target_n_nodes,
                                               input->set_maker,
                                               input->seed_maker,
                                               input->fitness_function,
                                               input->index_type);
  while(!krapivsky_done(km, input->target_n_nodes))
    krapivsky_next(km,
                   input->node_adder,
                   input->in_degree_sampler,
                   input->out_degree_sampler,
                   input->new_node_in_degree_indexer,
                   input->new_node_out_degree_indexer,
                   input->existing_node_in_degree_indexer,
                   input->existing_node_out_degree_indexer);
  return km;
}

void krapivsky_write_edges(krapivsky_model_t *km, char *filename) {
  uint64_t i;
  char *edge_file_name;
  FILE *edge_file;
  node_list_t *edge;

  edge_file_name = filename;
  edge_file = fopen(edge_file_name,"w");
  
  for (i=0;i<km->n_nodes;i++) {
    edge = km->nodes[i]->adjacency_list;
    while(edge != NULL) {
      fprintf(edge_file,"%llu,%llu\n",km->nodes[i]->id,edge->node->id);
      edge = edge->next;
    }
  }
  fclose(edge_file);
}

void krapivsky_write_degrees(krapivsky_model_t *km, char *filename) {
  uint64_t i;
  FILE *file;

  file = fopen(filename,"w");
  for(i = 0; i<km->n_nodes; i++)
    fprintf(file,"%llu,%llu\n",km->nodes[i]->out_degree,km->nodes[i]->in_degree);
  fclose(file);
}

void
krapivsky_free(krapivsky_model_t *km) {
  uint64_t i;

  // free indexes
  // this is fragile; detect type union by looking at size of
  // structure pointed to
  if(km->index_type == 0) {
    bstreap_free(km->in_degree_set.bstreap);
    bstreap_free(km->out_degree_set.bstreap);
  } else {
    heap_free(km->in_degree_set.heap);
    heap_free(km->out_degree_set.heap);
  }

  // free nodes
  for (i = 0; i<km->n_nodes; i++)
    node_free(km->nodes[i]);
  free(km->nodes);

  // free model
  free(km);
}

// abstraction providers
void
make_krapivsky_bstreaps(krapivsky_model_t *km) {
  km->in_degree_set.bstreap = make_bstreap();
  km->out_degree_set.bstreap = make_bstreap();
}

void
make_krapivsky_heaps(krapivsky_model_t *km) {
  km->in_degree_set.heap = make_heap();
  km->out_degree_set.heap = make_heap();
}

node_t *
krapivsky_bstreap_in_degree_sampler(krapivsky_model_t *km) {
  return bstreap_sample(km->in_degree_set.bstreap);
}

node_t *
krapivsky_bstreap_out_degree_sampler(krapivsky_model_t *km) {
  return bstreap_sample(km->out_degree_set.bstreap);
}

node_t *
krapivsky_heap_linear_in_degree_sampler(krapivsky_model_t *km) {
  return heap_sample_increment_linear(km->in_degree_set.heap);
}

node_t *
krapivsky_heap_linear_out_degree_sampler(krapivsky_model_t *km) {
  return heap_sample_increment_linear(km->out_degree_set.heap);
}

node_t *
krapivsky_heap_quadratic_in_degree_sampler(krapivsky_model_t *km) {
  return heap_sample_increment_quadratic_in_degree(km->in_degree_set.heap);
}

node_t *
krapivsky_heap_quadratic_out_degree_sampler(krapivsky_model_t *km) {
  return heap_sample_increment_quadratic_out_degree(km->out_degree_set.heap);
}

void 
krapivsky_bstreap_in_degree_indexer_lnu(krapivsky_model_t *km, node_t *node) {
  bstreap_in_degree_insert_lnu(km->in_degree_set.bstreap, node);
}

void 
krapivsky_bstreap_out_degree_indexer_lnu(krapivsky_model_t *km, node_t *node) {
  bstreap_out_degree_insert_lnu(km->out_degree_set.bstreap, node);
}

void 
krapivsky_bstreap_in_degree_indexer_lnn(krapivsky_model_t *km, node_t *node) {
  bstreap_in_degree_insert_lnn(km->in_degree_set.bstreap, node);
}

void 
krapivsky_bstreap_out_degree_indexer_lnn(krapivsky_model_t *km, node_t *node) {
  bstreap_out_degree_insert_lnn(km->out_degree_set.bstreap, node);
}

void 
krapivsky_bstreap_in_degree_indexer_lsu(krapivsky_model_t *km, node_t *node) {
  bstreap_in_degree_insert_lsu(km->in_degree_set.bstreap, node);
}

void 
krapivsky_bstreap_out_degree_indexer_lsu(krapivsky_model_t *km, node_t *node) {
  bstreap_out_degree_insert_lsu(km->out_degree_set.bstreap, node);
}

void 
krapivsky_bstreap_in_degree_indexer_lsn(krapivsky_model_t *km, node_t *node) {
  bstreap_in_degree_insert_lsn(km->in_degree_set.bstreap, node);
}

void 
krapivsky_bstreap_out_degree_indexer_lsn(krapivsky_model_t *km, node_t *node) {
  bstreap_out_degree_insert_lsn(km->out_degree_set.bstreap, node);
}

void
krapivsky_null_indexer(krapivsky_model_t *km, node_t *node) {
  return;
}

void 
krapivsky_heap_in_degree_linear_indexer(krapivsky_model_t *km, node_t *node) {
  heap_in_degree_linear_insert(km->in_degree_set.heap, node);
}

void
krapivsky_heap_out_degree_linear_indexer(krapivsky_model_t *km, node_t *node) {
  heap_out_degree_linear_insert(km->out_degree_set.heap, node);
}

void 
krapivsky_heap_in_degree_quadratic_indexer(krapivsky_model_t *km, node_t *node) {
  heap_in_degree_quadratic_insert(km->in_degree_set.heap, node);
}

void
krapivsky_heap_out_degree_quadratic_indexer(krapivsky_model_t *km, node_t *node) {
  heap_out_degree_quadratic_insert(km->out_degree_set.heap, node);
}

void
krapivsky_bstreap_node_adder_lnu(krapivsky_model_t *km, node_t *new_node) {
  bstreap_in_degree_insert_lnu(km->in_degree_set.bstreap, new_node);
  bstreap_out_degree_insert_lnu(km->out_degree_set.bstreap, new_node);
}

void
krapivsky_bstreap_node_adder_lnn(krapivsky_model_t *km, node_t *new_node) {
  bstreap_in_degree_insert_lnn(km->in_degree_set.bstreap, new_node);
  bstreap_out_degree_insert_lnn(km->out_degree_set.bstreap, new_node);
}

void
krapivsky_bstreap_node_adder_lsu(krapivsky_model_t *km, node_t *new_node) {
  bstreap_in_degree_insert_lsu(km->in_degree_set.bstreap, new_node);
  bstreap_out_degree_insert_lsu(km->out_degree_set.bstreap, new_node);
}

void
krapivsky_bstreap_node_adder_lsn(krapivsky_model_t *km, node_t *new_node) {
  bstreap_in_degree_insert_lsn(km->in_degree_set.bstreap, new_node);
  bstreap_out_degree_insert_lsn(km->out_degree_set.bstreap, new_node);
}

void
krapivsky_heap_linear_node_adder(krapivsky_model_t *km, node_t *new_node) {
  heap_in_degree_linear_insert(km->in_degree_set.heap, new_node);
  heap_out_degree_linear_insert(km->out_degree_set.heap, new_node);
}

void
krapivsky_heap_quadratic_node_adder(krapivsky_model_t *km, node_t *new_node) {
  heap_in_degree_quadratic_insert(km->in_degree_set.heap, new_node);
  heap_out_degree_quadratic_insert(km->out_degree_set.heap, new_node);
}

// curried functions
krapivsky_input_t *
krapivsky_make_input(double p,
                     double lambda,
                     double mu,
                     uint64_t target_n_nodes) {
  krapivsky_input_t *input = (krapivsky_input_t*) malloc(sizeof(krapivsky_input_t));
  if(!input) return 0;
  input->p = p;
  input->lambda = lambda;
  input->mu = mu;
  input->target_n_nodes = target_n_nodes;

  // defaults
  // constant fitness
  input->fitness_function = identity;
  
  return input;
}

void
krapivsky_input_bstreap(krapivsky_input_t *input) {
  input->set_maker = make_krapivsky_bstreaps;
  input->index_type = 0;
  input->in_degree_sampler = krapivsky_bstreap_in_degree_sampler;
  input->out_degree_sampler = krapivsky_bstreap_out_degree_sampler;
}

void
krapivsky_input_heap(krapivsky_input_t *input) {
  input->set_maker = make_krapivsky_heaps;
  // by defualt: linear
  input->seed_maker = make_heap_item_seed_linear;
  input->index_type = 1;
  input->in_degree_sampler = krapivsky_heap_linear_in_degree_sampler;
  input->out_degree_sampler = krapivsky_heap_linear_out_degree_sampler;
  // by defualt: linear
  input->node_adder = krapivsky_heap_linear_node_adder;
  // by defualt: linear
  input->new_node_in_degree_indexer  = krapivsky_heap_in_degree_linear_indexer;
  input->new_node_out_degree_indexer = krapivsky_heap_out_degree_linear_indexer;
  input->existing_node_in_degree_indexer  = krapivsky_null_indexer;
  input->existing_node_out_degree_indexer = krapivsky_null_indexer;
}

void
krapivsky_input_quadratic_heap(krapivsky_input_t *input) {
  input->seed_maker = make_heap_item_seed_quadratic;
  input->node_adder = krapivsky_heap_quadratic_node_adder;
  input->new_node_in_degree_indexer  = krapivsky_heap_in_degree_quadratic_indexer;
  input->new_node_out_degree_indexer = krapivsky_heap_out_degree_quadratic_indexer;
  input->in_degree_sampler = krapivsky_heap_quadratic_in_degree_sampler;
  input->out_degree_sampler = krapivsky_heap_quadratic_out_degree_sampler;
}

void
krapivsky_input_pareto(krapivsky_input_t *input) {
  input->fitness_function = sample_fitness_pareto;
}

krapivsky_model_t *
krapivsky_bstreap_simulate_lnu(krapivsky_input_t *input) {
  krapivsky_model_t *km;
  krapivsky_input_bstreap(input);
  input->seed_maker = make_bstreap_item_seed_lnu;
  input->node_adder = krapivsky_bstreap_node_adder_lnu;
  input->new_node_in_degree_indexer  = krapivsky_bstreap_in_degree_indexer_lnu;
  input->new_node_out_degree_indexer = krapivsky_bstreap_out_degree_indexer_lnu;
  input->existing_node_in_degree_indexer  = input->new_node_in_degree_indexer;
  input->existing_node_out_degree_indexer = input->new_node_out_degree_indexer;
  km = krapivsky_simulate(input);
  //free(input);
  return km;
}

krapivsky_model_t *
krapivsky_bstreap_simulate_lnn(krapivsky_input_t *input) {
  krapivsky_model_t *km;
  krapivsky_input_bstreap(input);
  input->seed_maker = make_bstreap_item_seed_lnn;
  input->node_adder = krapivsky_bstreap_node_adder_lnn;
  input->new_node_in_degree_indexer  = krapivsky_bstreap_in_degree_indexer_lnn;
  input->new_node_out_degree_indexer = krapivsky_bstreap_out_degree_indexer_lnn;
  input->existing_node_in_degree_indexer  = input->new_node_in_degree_indexer;
  input->existing_node_out_degree_indexer = input->new_node_out_degree_indexer;
  km = krapivsky_simulate(input);
  //free(input);
  return km;
}

krapivsky_model_t *
krapivsky_bstreap_simulate_lsu(krapivsky_input_t *input) {
  krapivsky_model_t *km;
  krapivsky_input_bstreap(input);
  input->seed_maker = make_bstreap_item_seed_lsu;
  input->node_adder = krapivsky_bstreap_node_adder_lsu;
  input->new_node_in_degree_indexer  = krapivsky_bstreap_in_degree_indexer_lsu;
  input->new_node_out_degree_indexer = krapivsky_bstreap_out_degree_indexer_lsu;
  input->existing_node_in_degree_indexer  = input->new_node_in_degree_indexer;
  input->existing_node_out_degree_indexer = input->new_node_out_degree_indexer;
  km = krapivsky_simulate(input);
  //free(input);
  return km;
}

krapivsky_model_t *
krapivsky_bstreap_simulate_lsn(krapivsky_input_t *input) {
  krapivsky_model_t *km;
  krapivsky_input_bstreap(input);
  input->seed_maker = make_bstreap_item_seed_lsn;
  input->node_adder = krapivsky_bstreap_node_adder_lsn;
  input->new_node_in_degree_indexer  = krapivsky_bstreap_in_degree_indexer_lsn;
  input->new_node_out_degree_indexer = krapivsky_bstreap_out_degree_indexer_lsn;
  input->existing_node_in_degree_indexer  = input->new_node_in_degree_indexer;
  input->existing_node_out_degree_indexer = input->new_node_out_degree_indexer;
  km = krapivsky_simulate(input);
  //free(input);
  return km;
}

krapivsky_model_t *krapivsky_heap_simulate(krapivsky_input_t *input) {
  krapivsky_model_t *km;
  krapivsky_input_heap(input);
  km = krapivsky_simulate(input);
  //free(input);
  return km;
}

krapivsky_model_t *
krapivsky_bstreap_simulate_pareto_lnu(krapivsky_input_t *input) {
  krapivsky_input_pareto(input);
  return krapivsky_bstreap_simulate_lnu(input);
}

krapivsky_model_t *
krapivsky_bstreap_simulate_pareto_lnn(krapivsky_input_t *input) {
  krapivsky_input_pareto(input);
  return krapivsky_bstreap_simulate_lnn(input);
}

krapivsky_model_t *
krapivsky_bstreap_simulate_pareto_lsu(krapivsky_input_t *input) {
  krapivsky_input_pareto(input);
  return krapivsky_bstreap_simulate_lsu(input);
}

krapivsky_model_t *
krapivsky_bstreap_simulate_pareto_lsn(krapivsky_input_t *input) {
  krapivsky_input_pareto(input);
  return krapivsky_bstreap_simulate_lsn(input);
}

krapivsky_model_t
*krapivsky_heap_simulate_pareto(krapivsky_input_t *input) {
  krapivsky_input_pareto(input);
  return krapivsky_heap_simulate(input);
}

krapivsky_model_t
*krapivsky_heap_simulate_quadratic(krapivsky_input_t *input) {
  krapivsky_model_t *km;
  krapivsky_input_heap(input);
  krapivsky_input_quadratic_heap(input);
  km = krapivsky_simulate(input);
  return km;
}

//macro-defined
MAKE_KRAPIV_PREF_FUNCTIONS(alpha_100)
MAKE_KRAPIV_PREF_FUNCTIONS(alpha_101)
MAKE_KRAPIV_PREF_FUNCTIONS(alpha_102)
MAKE_KRAPIV_PREF_FUNCTIONS(alpha_103)
MAKE_KRAPIV_PREF_FUNCTIONS(alpha_104)
MAKE_KRAPIV_PREF_FUNCTIONS(alpha_105)
MAKE_KRAPIV_PREF_FUNCTIONS(alpha_110)
MAKE_KRAPIV_PREF_FUNCTIONS(alpha_115)
MAKE_KRAPIV_PREF_FUNCTIONS(alpha_120)
MAKE_KRAPIV_PREF_FUNCTIONS(alpha_140)
MAKE_KRAPIV_PREF_FUNCTIONS(alpha_160)
MAKE_KRAPIV_PREF_FUNCTIONS(alpha_180)
MAKE_KRAPIV_PREF_FUNCTIONS(alpha_200)
