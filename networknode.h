#ifndef QUICKNET_NETWORKNODE_H
#define QUICKNET_NETWORKNODE_H


typedef struct node node_t;
typedef struct node_list node_list_t;

struct node {
  uint64_t id;
  uint64_t in_degree;
  uint64_t out_degree;
  double lambda;
  double mu;
  node_list_t *adjacency_list;
};

struct node_list {
  node_t *node;
  node_list_t *next;
};


node_t *make_node(uint64_t id, double lambda, double mu);
//node_list_t *make_node_list();
node_list_t *make_node_list(node_t *n);
void add_edge(node_t *n_from, node_t *n_to);

double get_in_degree(node_t *node);
double get_linear_in_degree(node_t *node);
double get_quadratic_in_degree(node_t *node);
double get_out_degree(node_t *node);
double get_linear_out_degree(node_t *node);
double get_quadratic_out_degree(node_t *node);

void node_free(node_t *node);
void node_list_free(node_list_t *node_list);
#endif /* QUICKNET_NETWORKNODE_H */
