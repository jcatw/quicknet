#ifndef QUICKNET_UTIL_H
#define QUICKNET_UTIL_H

#include <math.h>

#define def_identity_fn(type) \
  type identity_ ##type (type x) { \
    return x; \
  }

#define def_alpha_preference_fns(alpha,name) \
  double compute_ ## name ## _preference_mass_in(directed_node_t *node) { \
    return pow(node->in_degree, alpha) + node->lambda;                  \
  }                                                                     \
  double compute_ ##name## _preference_mass_out(directed_node_t *node) { \
    return pow(node->out_degree, alpha) + node->mu;                     \
  }                                                                     \
  double compute_ ##name## _increased_mass_in(directed_node_t *node) {  \
    directed_node_t *increased_node;                                    \
    increased_node = clone_directed_node(node);                         \
    increased_node->in_degree++;                                        \
    return compute_ ##name ## _preference_mass_in(increased_node);      \
  }                                                                     \
  double compute_ ##name## _increased_mass_out(directed_node_t *node) { \
    directed_node_t *increased_node;                                    \
    increased_node = clone_directed_node(node);                         \
    increased_node->out_degree++;                                       \
    return compute_ ##name ## _preference_mass_out(increased_node);     \
  }

#endif /* QUICKNET_UTIL_H */
