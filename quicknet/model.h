#ifndef QUICKNET_MODEL_H
#define QUICKNET_MODEL_H

#include "types.h"

struct directed_model {
  heap_t *in_degree_heap;
  heap_t *out_degree_heap;
  uint64_t n_nodes;
  uint64_t target_n_nodes;
  double p;
  double lambda;
  double mu;
  double (*fitness_function_in)(double fitness);
  double (*fitness_function_out)(double fitness);
  double (*compute_preference_mass_in) (directed_node_t *node);
  double (*compute_preference_mass_out)(directed_node_t *node);
  double (*compute_increased_mass_in) (directed_node_t *node);
  double (*compute_increased_mass_out)(directed_node_t *node);
  //void (*node_adder) (directed_model_t *dm, directed_node_t *new_node);
  //directed_node_t* (*in_degree_sampler) (directed_model_t *dt);
  //directed_node_t* (*out_degree_sampler) (directed_model_t *dt);
  //void (*in_degree_indexer) (directed_model_t *dt, node_t *node);
  //void (*out_degree_indexer) (directed_model_t *dt, node_t *node;
  directed_node_t **nodes;
};

directed_model_t *make_directed_model(double p,
                                      double lambda,
                                      double mu,
                                      uint64_t target_n_nodes,
                                      double (*fitness_function_in)(double fitness),
                                      double (*fitness_function_out)(double fitness),
                                      double (*compute_preference_mass_in) (directed_node_t *node),
                                      double (*compute_preference_mass_out)(directed_node_t *node),
                                      double (*compute_increased_mass_in) (directed_node_t *node),
                                      double (*compute_increased_mass_out)(directed_node_t *node));
void reset_model(directed_model_t *dm);
void directed_seed(directed_model_t *dm);
void write_directed_network_edges(directed_model_t *dm, char *filename);
#endif /* QUICKNET_MODEL_H */
