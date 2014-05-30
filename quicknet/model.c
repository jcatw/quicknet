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
  directed_model_t *dm = (directed_model_t *) malloc(sizeof(*dm));
  if (!dm) {
    fprintf(stderr, "Out of memory.\n");
    exit(1);
  }
  dm->in_degree_heap = make_heap();
  dm->out_degree_heap = make_heap();
  dm->n_nodes = 0;
  dm->target_n_nodes = target_n_nodes;
  dm->p = p;
  dm->lambda = lambda;
  dm->mu = mu;
  dm->fitness_function_in = fitness_function_in;
  dm->fitness_function_out = fitness_function_out;
  dm->compute_preference_mass_in  = compute_preference_mass_in;
  dm->compute_preference_mass_out = compute_preference_mass_out;
  dm->compute_increased_mass_in  = compute_increased_mass_in;
  dm->compute_increased_mass_out = compute_increased_mass_out;
  dm->nodes = (directed_node_t **) malloc(dm->target_n_nodes * sizeof(directed_node_t*));
  if (!dm->nodes) {
    fprintf(stderr, "Out of memory.\n");
    exit(1);
  }
  directed_seed(dm);
  return dm;
}

void reset_model(directed_model_t *dm) {
  uint64_t i = 0;
  
  heap_free(dm->in_degree_heap);
  heap_free(dm->out_degree_heap);
  dm->in_degree_heap = make_heap();
  dm->out_degree_heap = make_heap();
  
  for(i = 0; i < dm->n_nodes; i++) {
    directed_node_free(dm->nodes[i], 1);
  }

  dm->n_nodes = 0;
  directed_seed(dm);
}

void directed_seed(directed_model_t *dm) {
  directed_node_t *node0, *node1, *node2;

  node0 = make_directed_node(0,
                             dm->fitness_function_in(dm->lambda),
                             dm->fitness_function_out(dm->mu));
  node1 = make_directed_node(1,
                             dm->fitness_function_in(dm->lambda),
                             dm->fitness_function_out(dm->mu));
  node2 = make_directed_node(2,
                             dm->fitness_function_in(dm->lambda),
                             dm->fitness_function_out(dm->mu));

  dm->nodes[0] = node0;
  dm->nodes[1] = node1;
  dm->nodes[2] = node2;

  dm->n_nodes = 3;

  add_directed_edge(node0, node1);
  add_directed_edge(node0, node2);
  add_directed_edge(node1, node2);

  heap_insert(dm->in_degree_heap, node0, dm->compute_preference_mass_in);
  heap_insert(dm->in_degree_heap, node1, dm->compute_preference_mass_in);
  heap_insert(dm->in_degree_heap, node2, dm->compute_preference_mass_in);

  heap_insert(dm->out_degree_heap, node0, dm->compute_preference_mass_out);
  heap_insert(dm->out_degree_heap, node1, dm->compute_preference_mass_out);
  heap_insert(dm->out_degree_heap, node2, dm->compute_preference_mass_out);
}

void write_directed_network_edges(directed_model_t *dm, char *filename) {
  uint64_t i;
  FILE *edge_file;
  directed_node_list_t *edge;

  edge_file = fopen(filename,"w");
  for(i=0;i<dm->n_nodes;i++) {
    edge = dm->nodes[i]->adjacency_list;
    while(edge != NULL) {
      fprintf(edge_file,"%llu,%llu\n",dm->nodes[i]->id,edge->node->id);
      edge = edge->next;
    }
  }
  fclose(edge_file);
}

