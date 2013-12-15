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

#define MAKE_NODE_PREF_HEADER(name)                                     \
  double get_ ## name ## _in_degree(node_t *node);                      \
  double get_ ## name ## _out_degree(node_t *node);                     

#define MAKE_NODE_PREF_FUNCTIONS(name, alphavalue) \
  double get_ ## name ## _in_degree(node_t *node) {                     \
  return pow(node->in_degree, alphavalue) + node->lambda;                 \
  }                                                                     \
  double get_ ## name ## _out_degree(node_t *node) {                    \
  return pow(node->out_degree, alphavalue) + node->mu;                    \
  }                                                                     
  
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

// macro-defined
//MAKE_NODE_PREF_HEADER(alpha_100)
//MAKE_NODE_PREF_HEADER(alpha_101)
//MAKE_NODE_PREF_HEADER(alpha_102)
//MAKE_NODE_PREF_HEADER(alpha_103)
//MAKE_NODE_PREF_HEADER(alpha_104)
//MAKE_NODE_PREF_HEADER(alpha_105)
//MAKE_NODE_PREF_HEADER(alpha_110)
//MAKE_NODE_PREF_HEADER(alpha_115)
//MAKE_NODE_PREF_HEADER(alpha_120)
//MAKE_NODE_PREF_HEADER(alpha_140)
//MAKE_NODE_PREF_HEADER(alpha_160)
//MAKE_NODE_PREF_HEADER(alpha_180)
//MAKE_NODE_PREF_HEADER(alpha_200)
#endif /* QUICKNET_NETWORKNODE_H */
