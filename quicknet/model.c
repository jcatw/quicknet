#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "networknode.h"
#include "heap.h"
#include "model.h"

directed_model_t *make_directed_model(double p,
                                      double lambda,
                                      double mu,
                                      uint64_t target_n_nodes,
                                      double (*fitness_function_in)(double fitness),
                                      double (*fitness_function_out)(double fitness),
                                      double (*compute_preference_mass_in) (directed_node_t *node),
                                      double (*compute_preference_mass_out)(directed_node_t *node),
                                      double (*compute_increased_mass_in) (directed_node_t *node),
                                      double (*compute_increased_mass_out)(directed_node_t *node)) {
                                      //void (*node_adder) (directed_model_t *dm, directed_node_t *new_node),
                                      //directed_node_t* (*in_degree_sampler) (directed_model_t *dt),
                                      //directed_node_t* (*out_degree_sampler) (directed_model_t *dt),
                                      //void (*in_degree_indexer) (directed_model_t *dt, node_t *node),
                                      //void (*out_degree_indexer) (directed_model_t *dt, node_t *node)) {
  directed_model_t *dm = (directed_model_t *) malloc(sizeof(*dm));
  if (!dm) {
    fprintf(stderr, "Out of memory.\n");
    exit(1);
  }
  dm->in_degree_heap = make_heap();
  dm->out_degree_heap = make_heap();
  dm->n_nodes = 0;
  dm->p = p;
  dm->lambda = lambda;
  dm->mu = mu;
  dm->fitness_function_in = fitness_function_in;
  dm->fitness_function_out = fitness_function_out;
  dm->compute_preference_mass_in  = compute_preference_mass_in;
  dm->compute_preference_mass_out = compute_preference_mass_out;
  dm->compute_increased_mass_in  = compute_increased_mass_in;
  dm->compute_increased_mass_out = compute_increased_mass_out;
  //dm->node_adder = node_adder;
  //dm->in_degree_sampler = in_degree_sampler;
  //dm->out_degree_sampler = out_degree_sampler;
  //dm->in_degree_indexer = in_degree_indexer;
  //dm->out_degree_indexer = out_degree_indexer;
  dm->nodes = (directed_node_t **) malloc(dm->n_nodes * sizeof(directed_node_t*));
  if (!dm->nodes) {
    fprintf(stderr, "Out of memory.\n");
    exit(1);
  }
  return dm;
}

