#include <stdlib.h>
#include <stdint.h>

#include "networknode.h"
#include "bstreap.h"
#include "krapivsky.h"

struct krapivsky_model {
  union {
    bstreap_t *bstreap;
  } *in_degree_set;
  union {
    bstreap_t *bstreap;
  } *out_degree_set;
  uint64_t n_nodes;
  double p;
  double lambda;
  double mu;
  node_t **nodes;
};

krapivsky_model_t *
make_krapivsky_model(double p,
                     double lambda,
                     double mu,
                     void  (*set_maker)(krapivsky_model_t*),
                     void  (*seed_maker)(krapivsky_model_t*)) {
  krapivsky_model_t *km = (krapivsky_model_t*) malloc(sizeof(*km));
  if (!km) return 0;
  (*set_maker)(km);
  km->p = p;
  km->lambda = lambda;
  km->mu = mu;
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
  bstreap_in_degree_insert_lnu(km->in_degree_set->bstreap,node0);
  bstreap_in_degree_insert_lnu(km->in_degree_set->bstreap,node1);
  bstreap_in_degree_insert_lnu(km->in_degree_set->bstreap,node2);
  bstreap_in_degree_insert_lnu(km->out_degree_set->bstreap,node0);
  bstreap_in_degree_insert_lnu(km->out_degree_set->bstreap,node1);
  bstreap_in_degree_insert_lnu(km->out_degree_set->bstreap,node2);
}

int8_t
krapivsky_done(krapivsky_model_t *km, uint64_t target_n_nodes) {
  return km->n_nodes >= target_n_nodes;
}

void
krapivsky_next(krapivsky_model_t *km,
               node_t* (*node_adder) (krapivsky_model_t *km),
               node_t* (*in_degree_sampler) (krapivsky_model_t *km),
               node_t* (*out_degree_sampler) (krapivsky_model_t *km),
               void (*in_degree_indexer) (krapivsky_model_t *km, node_t *node),
               void (*out_degree_indexer) (krapivsky_model_t *km, node_t *node)) {
  double uniform_sample;
  node_t *new_node, *in_degree_sampled_node, *out_degree_sampled_node;

  uniform_sample = rand() / (double)RAND_MAX;

  // node addition step
  if(uniform_sample < km->p) {
    // abstraction: node adder
    
    new_node = (*node_adder)(km);

    // abstraction: in degree sampler
    in_degree_sampled_node = (*in_degree_sampler)(km);

    // no further abstraction
    add_edge(new_node,in_degree_sampled_node);

    // abstraction: in, out degree indexers
    (*out_degree_indexer)(km, new_node);
    (*in_degree_indexer)(km, new_node);

    (*in_degree_indexer)(km, in_degree_sampled_node);
  }
  else { //edge step
    // abstraction: in, out degree samplers
    out_degree_sampled_node = (*out_degree_sampler)(km);
    in_degree_sampled_node = (*in_degree_sampler)(km);

    // no further abstraction
    add_edge(out_degree_sampled_node, in_degree_sampled_node);

    // abstraction: in, out degree indexers
    (*out_degree_indexer)(km, out_degree_sampled_node);
    (*in_degree_indexer)(km, in_degree_sampled_node);
  }
}

krapivsky_model_t*
krapivsky_simulate(double p,
                   double lambda,
                   double mu,
                   uint64_t target_n_nodes,
                   void  (*set_maker)(krapivsky_model_t*),
                   void  (*seed_maker)(krapivsky_model_t*),
                   node_t* (*node_adder) (krapivsky_model_t *km),
                   node_t* (*in_degree_sampler) (krapivsky_model_t *km),
                   node_t* (*out_degree_sampler) (krapivsky_model_t *km),
                   void (*in_degree_indexer) (krapivsky_model_t *km, node_t *node),
                   void (*out_degree_indexer) (krapivsky_model_t *km, node_t *node)) {
  krapivsky_model_t *km = make_krapivsky_model(p, lambda, mu, set_maker, seed_maker);
  while(!krapivsky_done(km, target_n_nodes))
    krapivsky_next(km,
                   node_adder,
                   in_degree_sampler,
                   out_degree_sampler,
                   in_degree_indexer,
                   out_degree_indexer);
  return km;
}

// abstraction providers
void
make_krapivsky_bstreaps(krapivsky_model_t *km) {
  km->in_degree_set->bstreap = make_bstreap();
  km->out_degree_set->bstreap = make_bstreap();
}

node_t *
krapivsky_bstreap_in_degree_sampler(krapivsky_model_t *km) {
  return bstreap_sample(km->in_degree_set->bstreap);
}

node_t *
krapivsky_bstreap_out_degree_sampler(krapivsky_model_t *km) {
  return bstreap_sample(km->out_degree_set->bstreap);
}

void 
krapivsky_bstreap_in_degree_indexer_lnu(krapivsky_model_t *km, node_t *node) {
  bstreap_in_degree_insert_lnu(km->in_degree_set->bstreap, node);
}

void 
krapivsky_bstreap_out_degree_indexer_lnu(krapivsky_model_t *km, node_t *node) {
  bstreap_out_degree_insert_lnu(km->out_degree_set->bstreap, node);
}

node_t *
krapivsky_bstreap_node_adder_lnu(krapivsky_model_t *km) {
  node_t *new_node;
  
  new_node = make_node(km->n_nodes,km->lambda,km->mu);
  km->nodes[km->n_nodes] = new_node;
  km->n_nodes++;

  bstreap_in_degree_insert_lnu(km->in_degree_set->bstreap, new_node);
  bstreap_out_degree_insert_lnu(km->out_degree_set->bstreap, new_node);

  return new_node;
}

// curried functions
//krapivsky_model_t *
//make_bstreap_krapivsky_model(double p,
//                             double lambda,
//                             double mu) {
//  return make_krapivsky_model(p,
//                              lambda,
//                              mu,
//                              make_krapivsky_bstreaps,
//                              make_bstreap_item_seed);
//}
krapivsky_model_t *
krapivsky_bstreap_simulate_lnu(double p,
                               double lambda,
                               double mu,
                               uint64_t target_n_nodes) {
  return krapivsky_simulate(p,
                            lambda,
                            mu,
                            target_n_nodes,
                            make_krapivsky_bstreaps,
                            make_bstreap_item_seed_lnu,
                            krapivsky_bstreap_node_adder_lnu,
                            krapivsky_bstreap_in_degree_sampler,
                            krapivsky_bstreap_out_degree_sampler,
                            krapivsky_bstreap_in_degree_indexer_lnu,
                            krapivsky_bstreap_out_degree_indexer_lnu);
}
