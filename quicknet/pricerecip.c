#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "clparse.h"
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

void simulate_reciprocated_price_model(directed_model_t *price_model, double r);

int main(int argc, char** argv) {
  clopts_t *options = (clopts_t*) malloc(sizeof(*options));
  directed_model_t *price_model_linear;

  srand(time(NULL) ^ (getpid()<<16));

  // set default options
  options->target_n_nodes = 10000;
  options->lambda = 1.0;
  options->mu = 0.0;
  options->r = 0.2;  //probability of reciprocation
  strcpy(options->edge_file_name,"price_edges.csv");

  // parse command line options
  clparse(options, argc, argv);

  // construct model
  printf("Building model.\n");
  price_model_linear = make_directed_model(options->p,
                                           options->lambda,
                                           options->mu,
                                           options->target_n_nodes,
                                           identity_double,
                                           identity_double,
                                           compute_linear_preference_mass_in,
                                           compute_linear_preference_mass_out,
                                           compute_linear_increased_mass_in,
                                           compute_linear_increased_mass_out);

  // simulate network
  printf("Simulating network.\n");
  simulate_reciprocated_price_model(price_model_linear, options->r);

  // write edges
  printf("Writing edges to %s.\n",options->edge_file_name);
  write_directed_network_edges(price_model_linear, options->edge_file_name);

  printf("Cleaning Up.\n");
  free(options);

  printf("Done.\n");
  return 0;
}

void simulate_reciprocated_price_model(directed_model_t *price_model, double r) {
  double u;
  uint64_t i;
  directed_node_t *new_node, *sampled_node;

  for(i=price_model->n_nodes; i<price_model->target_n_nodes; i++) {
    // sample an existing node by in-degree and increase its preference mass
    sampled_node = heap_sample_increment(price_model->in_degree_heap,
                                         price_model->compute_increased_mass_in);
    // introduce a new node to the network
    new_node = make_directed_node(price_model->n_nodes,
                                  price_model->fitness_function_in(price_model->lambda),
                                  price_model->mu); //out degree fitness irrelevant
    price_model->nodes[i] = new_node;
    price_model->n_nodes++;
    
    // add an edge from new node to the existing node
    add_directed_edge(new_node, sampled_node);
    // add a reciprocal edge with probability r
    u = rand() / (double) RAND_MAX;
    if (u < r) {
      add_directed_edge(sampled_node, new_node);
    }

    // index the new node
    heap_insert(price_model->in_degree_heap,
                new_node,
                price_model->compute_preference_mass_in);
  }
}
