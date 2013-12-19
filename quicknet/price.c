#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "util.h"
#include "networknode.h"
#include "heap.h"
#include "model.h"

// define identity_double
def_identity_fn(double);

// define:
//   compute_linear_preference_mass_{in,out}
//   compute_linear_increased_mass_{in,out}
def_alpha_preference_fns(1.0, linear); 

void simulate_price_model(directed_model_t *price_model);

int main(int argc, char** argv) {
  uint64_t n_nodes = 10000;
  double p = 0.;
  double lambda = 1.0;
  double mu = 0.;
  char* edge_file_name = "price_edges.csv";
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

  printf("Writing edges to %s.\n",edge_file_name);
  write_directed_network_edges(price_model_linear, edge_file_name);

  printf("Done.\n");
  return 0;
}

void simulate_price_model(directed_model_t *price_model) {
  uint64_t i;
  directed_node_t *new_node, *sampled_node;

  for(i=price_model->n_nodes; i<price_model->target_n_nodes; i++) {
    sampled_node = heap_sample_increment(price_model->in_degree_heap,
                                         price_model->compute_increased_mass_in);
    new_node = make_directed_node(price_model->n_nodes,
                                  price_model->fitness_function_in(price_model->lambda),
                                  price_model->mu); //out degree fitness irrelevant
    price_model->n_nodes++;
    price_model->nodes[i] = new_node;
    
    add_directed_edge(new_node, sampled_node);
    heap_insert(price_model->in_degree_heap,
                new_node,
                price_model->compute_preference_mass_in);
  }
}
