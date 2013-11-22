#ifndef QUICKNET_HEAP_H
#define QUICKNET_HEAP_H

typedef struct heap heap_t;
typedef struct heap_item heap_item_t;

struct heap {
  double total_mass;
  uint64_t n_nodes;
  heap_item_t **items;
  uint64_t n_alloced;
};

struct heap_item {
  node_t *node;
  uint64_t index;
  //double priority;
  double node_mass;
  double subtree_mass;
};

#define MAKE_HEAP_PREF_HEADERS(name)                                    \
  void heap_in_degree_ ## name ## _insert(heap_t *heap, node_t *node);     \
  void heap_out_degree_ ## name ## _insert(heap_t *heap, node_t *node);    \
  node_t *heap_sample_increment_ ## name ## _in_degree(heap_t *heap);      \
  node_t *heap_sample_increment_ ## name ## _out_degree(heap_t *heap);     \
  double compute_ ## name ## _new_mass_in_degree(heap_item_t *item);       \
  double compute_ ## name ## _new_mass_out_degree(heap_item_t *item);      
  
  
heap_t *make_heap();
heap_item_t *make_heap_item(node_t *n,
                            double (*compute_mass) (node_t*),
                            double (*compute_priority) (heap_item_t*));
heap_item_t *heap_left  (heap_t *heap, heap_item_t *item);
heap_item_t *heap_right (heap_t *heap, heap_item_t *item);
heap_item_t *heap_parent(heap_t *heap, heap_item_t *item);
void heap_insert(heap_t *heap,
                 node_t* node,
                 double (*compute_mass) (node_t*),
                 double (*compute_priority) (heap_item_t*));
void heap_in_degree_linear_insert(heap_t *heap, node_t *node);
void heap_out_degree_linear_insert(heap_t *heap, node_t *node);
void heap_in_degree_quadratic_insert(heap_t *heap, node_t *node);
void heap_out_degree_quadratic_insert(heap_t *heap, node_t *node);
void heap_increase_priority(heap_t *heap,
                            heap_item_t* item,
                            double new_priority,
                            double (*compute_priority)(heap_item_t*),
                            void (*set_priority)(heap_item_t*,double));
void heap_exchange(heap_t *heap, heap_item_t *child, heap_item_t *parent);
node_t *heap_sample_increment(heap_t *heap, double (*compute_new_mass)(heap_item_t *item));
node_t *heap_sample_increment_linear(heap_t *heap);
node_t *heap_sample_increment_quadratic_in_degree(heap_t *heap);
node_t *heap_sample_increment_quadratic_out_degree(heap_t *heap);
node_t *heap_item_sample_increment(heap_t *heap,
                                   heap_item_t *item,
                                   double observed_mass,
                                   double uniform_sample,
                                   double (*compute_new_mass)(heap_item_t *item));
double compute_linear_new_mass(heap_item_t *item);
double compute_quadratic_new_mass_in_degree(heap_item_t *item);
double compute_quadratic_new_mass_out_degree(heap_item_t *item);
double heap_item_get_node_mass(heap_item_t *item);
void heap_item_set_node_mass(heap_item_t *item, double node_mass);
double heap_item_get_priority(heap_item_t *item);
void heap_item_set_priority(heap_item_t *item, double priority);

void heap_free(heap_t *heap);

//macro-defined
MAKE_HEAP_PREF_HEADERS(alpha_100)
MAKE_HEAP_PREF_HEADERS(alpha_101)
MAKE_HEAP_PREF_HEADERS(alpha_102)
MAKE_HEAP_PREF_HEADERS(alpha_103)
MAKE_HEAP_PREF_HEADERS(alpha_104)
MAKE_HEAP_PREF_HEADERS(alpha_105)
MAKE_HEAP_PREF_HEADERS(alpha_110)
MAKE_HEAP_PREF_HEADERS(alpha_115)
MAKE_HEAP_PREF_HEADERS(alpha_120)
MAKE_HEAP_PREF_HEADERS(alpha_140)
MAKE_HEAP_PREF_HEADERS(alpha_160)
MAKE_HEAP_PREF_HEADERS(alpha_180)
MAKE_HEAP_PREF_HEADERS(alpha_200)
#endif /* QUICKNET_HEAP_H */
