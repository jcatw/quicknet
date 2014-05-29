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

void simulate_krapivsky_model(directed_model_t *krapivsky_model);

int main(int argc, char** argv) {
  clopts_t *options = (clopts_t*) malloc(sizeof(*options));
  directed_model_t *krapivsky_model_linear;

  srand(time(NULL) ^ (getpid()<<16));

  // set default options
  options->target_n_nodes = 10000;
  options->lambda = 3.5;
  options->mu = 1.8;
  options->p = 0.2;
  strcpy(options->edge_file_name,"krapivsky_edges.csv");

  // parse command line options
  clparse(options, argc, argv);

  // construct model
  printf("Building model.\n");
  krapivsky_model_linear = make_directed_model(options->p,
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
  simulate_krapivsky_model(krapivsky_model_linear);

  // write edges
  printf("Writing edges to %s.\n",options->edge_file_name);
  write_directed_network_edges(krapivsky_model_linear, options->edge_file_name);

  printf("Cleaning Up.\n");
  free(options);

  printf("Done.\n");
  return 0;
}

void simulate_krapivsky_model(directed_model_t *krapivsky_model) {
  double u;
  directed_node_t *new_node, *in_degree_sampled_node, *out_degree_sampled_node;

  // while the network still needs to grow...
  while(krapivsky_model->n_nodes < krapivsky_model->target_n_nodes) {
    u = rand() / (double) RAND_MAX;
    // with probability p, take a node step
    if (u < krapivsky_model->p) {
      // sample an existing node by in-degree and increase its preference mass
      in_degree_sampled_node = heap_sample_increment(krapivsky_model->in_degree_heap,
                                                     krapivsky_model->compute_increased_mass_in);
      // introduce a new node to the network
      new_node = make_directed_node(krapivsky_model->n_nodes,
                                    krapivsky_model->fitness_function_in(krapivsky_model->lambda),
                                    krapivsky_model->mu);
      krapivsky_model->nodes[krapivsky_model->n_nodes] = new_node;
      krapivsky_model->n_nodes++;
      

      // add an edge from new node to the existing node
      add_directed_edge(new_node, in_degree_sampled_node);

      // index the new node
      heap_insert(krapivsky_model->in_degree_heap,
                  new_node,
                  krapivsky_model->compute_preference_mass_in);
      heap_insert(krapivsky_model->out_degree_heap,
                  new_node,
                  krapivsky_model->compute_preference_mass_out);
    }
    // with probability 1-p, take an edge step
    else {
      // sample two existing nodes: one by in-degree and one by out-degree
      // increase their preference masses
      in_degree_sampled_node = heap_sample_increment(krapivsky_model->in_degree_heap,
                                                     krapivsky_model->compute_increased_mass_in);
      out_degree_sampled_node = heap_sample_increment(krapivsky_model->out_degree_heap,
                                                      krapivsky_model->compute_increased_mass_out);

      // add an edge between the sampled nodes
      add_directed_edge(out_degree_sampled_node, in_degree_sampled_node);
    }
  }
}

