#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "util.h"
#include "networknode.h"
#include "heap.h"
#include "model.h"

def_identity_fn(double);
def_alpha_preference_fns(1.0, linear);
def_alpha_preference_fns(2.0, quadratic);

void simulate_price_model(directed_model_t *price_model);

int main(int argc, char** argv) {
  uint64_t n_nodes = 10000;
  double p = 0.;
  double lambda = 1.0;
  double mu = 0.;
  directed_model_t *price_model_linear;

  printf("Building model.\n");
  price_model_linear = make_directed_model(p,
                                           lambda,
                                           mu,
                                           n_nodes,
                                           identity_double,
                                           identity_double,
                                           compute_linear_preference_mass_in,
                                           compute_linear_preference_mass_out,
                                           compute_linear_increased_mass_in,
                                           compute_linear_increased_mass_out);

  printf("Simulating model.\n");
  simulate_price_model(price_model_linear);

  printf("Done.\n");
  return 0;
}

void simulate_price_model(directed_model_t *price_model) {
  uint64_t i;
  directed_node_t *new_node, *sampled_node;

  for(i=0; i<price_model->n_nodes; i++) {
    sampled_node = heap_sample_increment(price_model->in_degree_heap,
                                         price_model->compute_preference_mass_in);
    new_node = make_directed_node(price_model->n_nodes,
                                  price_model->fitness_function_in(price_model->lambda),
                                  price_model->mu); //out degree fitness irrelevant
    add_directed_edge(new_node, sampled_node);
    heap_insert(price_model->in_degree_heap,
                new_node,
                price_model->compute_increased_mass_in);
  }
}
