#ifndef QUICKNET_NETWORKNODE_H
#define QUICKNET_NETWORKNODE_H

typedef struct node node_t;
typedef struct node_list node_list_t;

node_t *make_node(uint64_t id, double lambda, double mu);
//node_list_t *make_node_list();
node_list_t *make_node_list(node_t *n);
void add_edge(node_t *n_from, node_t *n_to);

double get_in_degree(node_t *node);
double get_out_degree(node_t *node);
#endif /* QUICKNET_NETWORKNODE_H */
