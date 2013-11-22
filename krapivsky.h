#ifndef QUICKNET_KRAPIVSKY_H
#define QUICKNET_KRAPIVSKY_H

// this type encapsulates a single simulation of a
// particular intance of the krapivsky model
typedef struct krapivsky_model krapivsky_model_t;
// this union provides indirection over the implementation of
// node indexing
typedef union degree_set degree_set_t;
// this type is used to encapsulate the input to simulations so I stop breaking
// the interface every time I make a change, and to facilitate currying
typedef struct krapivsky_input krapivsky_input_t;

union degree_set {
  bstreap_t *bstreap;
  heap_t *heap;
};

struct krapivsky_model {
  // index_type:
  //  0: bstreap
  //  1: heap
  uint8_t index_type;
  degree_set_t in_degree_set;
  degree_set_t out_degree_set;
  uint64_t n_nodes;
  double p;
  double lambda;
  double mu;
  node_t **nodes;
};

struct krapivsky_input {
  double p;
  double lambda;
  double mu;
  uint64_t target_n_nodes;
  void  (*set_maker)(krapivsky_model_t*);
  void  (*seed_maker)(krapivsky_model_t*);
  double (*fitness_function)(double fitness);
  uint8_t index_type;
  void (*node_adder) (krapivsky_model_t *km, node_t *new_node);
  node_t* (*in_degree_sampler) (krapivsky_model_t *km);
  node_t* (*out_degree_sampler) (krapivsky_model_t *km);
  void (*new_node_in_degree_indexer) (krapivsky_model_t *km, node_t *node);
  void (*new_node_out_degree_indexer) (krapivsky_model_t *km, node_t *node);
  void (*existing_node_in_degree_indexer) (krapivsky_model_t *km,node_t *node);
  void (*existing_node_out_degree_indexer) (krapivsky_model_t *km, node_t *node);
};

// This architecture lead to a lot of repitition.  This macro defines
// the prototypes for all of the functions needed to add a new
// preference function.
#define MAKE_KRAPIV_PREF_HEADERS(name) \
  void krapivsky_input_ ## name ## _heap(krapivsky_input_t *inputt); \
  node_t *krapivsky_heap_ ## name ## _in_degree_sampler(krapivsky_model_t *km); \
  node_t *krapivsky_heap_ ## name ## _out_degree_sampler(krapivsky_model_t *km); \
  void krapivsky_heap_in_degree_ ## name ## _indexer(krapivsky_model_t *km, node_t *node); \
  void krapivsky_heap_out_degree_ ## name ## _indexer(krapivsky_model_t *km, node_t *node); \
  krapivsky_model_t *krapivsky_heap_simulate_ ## name(krapivsky_input_t *input);

// general functions
krapivsky_model_t *make_krapivsky_model(double p,
                                        double lambda,
                                        double mu,
                                        uint64_t target_n_nodes,
                                        void  (*set_maker)(krapivsky_model_t*),
                                        void  (*seed_maker)(krapivsky_model_t*),
                                        double (*fitness_function)(double fitness),
                                        uint8_t index_type);
void make_bstreap_item_seed_lnu(krapivsky_model_t *km);
void make_bstreap_item_seed_lnn(krapivsky_model_t *km);
void make_bstreap_item_seed_lsu(krapivsky_model_t *km);
void make_bstreap_item_seed_lsn(krapivsky_model_t *km);
void make_heap_item_seed_linear(krapivsky_model_t *km);
void make_heap_item_seed_quadratic(krapivsky_model_t *km);
int8_t krapivsky_done(krapivsky_model_t *km, uint64_t target_n_nodes);
void krapivsky_next(krapivsky_model_t *km,
                    void (*node_adder) (krapivsky_model_t *km, node_t *new_node),
                    node_t* (*in_degree_sampler) (krapivsky_model_t *km),
                    node_t* (*out_degree_sampler) (krapivsky_model_t *km),
                    void (*new_node_in_degree_indexer) (krapivsky_model_t *km, node_t *node),
                    void (*new_node_out_degree_indexer) (krapivsky_model_t *km, node_t *node),
                    void (*existing_node_in_degree_indexer) (krapivsky_model_t *km, node_t *node),
                    void (*existing_node_out_degree_indexer) (krapivsky_model_t *km, node_t *node));
krapivsky_model_t* krapivsky_simulate(krapivsky_input_t *input);
void krapivsky_write_edges(krapivsky_model_t *km, char *filename);
void krapivsky_write_degrees(krapivsky_model_t *km, char *filename);
void krapivsky_free(krapivsky_model_t *km);

// abstraction providers
void make_krapivsky_bstreaps(krapivsky_model_t *km);
void make_krapivsky_heaps(krapivsky_model_t *km);
node_t *krapivsky_bstreap_in_degree_sampler(krapivsky_model_t *km);
node_t *krapivsky_bstreap_out_degree_sampler(krapivsky_model_t *km);
node_t *krapivsky_heap_linear_in_degree_sampler(krapivsky_model_t *km);
node_t *krapivsky_heap_linear_out_degree_sampler(krapivsky_model_t *km);
node_t *krapivsky_heap_quadratic_in_degree_sampler(krapivsky_model_t *km);
node_t *krapivsky_heap_quadratic_out_degree_sampler(krapivsky_model_t *km);
void krapivsky_bstreap_in_degree_indexer_lnu(krapivsky_model_t *km, node_t *node);
void krapivsky_bstreap_out_degree_indexer_lnu(krapivsky_model_t *km, node_t *node);
void krapivsky_bstreap_in_degree_indexer_lnn(krapivsky_model_t *km, node_t *node);
void krapivsky_bstreap_out_degree_indexer_lnn(krapivsky_model_t *km, node_t *node);
void krapivsky_bstreap_in_degree_indexer_lsu(krapivsky_model_t *km, node_t *node);
void krapivsky_bstreap_out_degree_indexer_lsu(krapivsky_model_t *km, node_t *node);
void krapivsky_bstreap_in_degree_indexer_lsn(krapivsky_model_t *km, node_t *node);
void krapivsky_bstreap_out_degree_indexer_lsn(krapivsky_model_t *km, node_t *node);
void krapivsky_null_indexer(krapivsky_model_t *km, node_t *node);
void krapivsky_heap_in_degree_indexer(krapivsky_model_t *km, node_t *node);
void krapivsky_heap_out_degree_indexer(krapivsky_model_t *km, node_t *node);
void krapivsky_heap_in_degree_linear_indexer(krapivsky_model_t *km, node_t *node);
void krapivsky_heap_out_degree_quadratic_indexer(krapivsky_model_t *km, node_t *node);
void krapivsky_heap_in_degree_quadratic_indexer(krapivsky_model_t *km, node_t *node);
void krapivsky_heap_out_degree_linear_indexer(krapivsky_model_t *km, node_t *node);
void krapivsky_bstreap_node_adder_lnu(krapivsky_model_t *km, node_t *new_node);
void krapivsky_bstreap_node_adder_lnn(krapivsky_model_t *km, node_t *new_node);
void krapivsky_bstreap_node_adder_lsu(krapivsky_model_t *km, node_t *new_node);
void krapivsky_bstreap_node_adder_lsn(krapivsky_model_t *km, node_t *new_node);
void krapivsky_heap_linear_node_adder(krapivsky_model_t *km, node_t *new_node);
void krapivsky_heap_quadratic_node_adder(krapivsky_model_t *km, node_t *new_node);

// curried functions
krapivsky_input_t *krapivsky_make_input(double p,
                                        double lambda,
                                        double mu,
                                        uint64_t target_n_nodes);
void krapivsky_input_bstreap(krapivsky_input_t *input);
void krapivsky_input_heap(krapivsky_input_t *input);
void krapivsky_input_pareto(krapivsky_input_t *input);
krapivsky_model_t *krapivsky_bstreap_simulate_lnu(krapivsky_input_t *input);
krapivsky_model_t *krapivsky_bstreap_simulate_lnn(krapivsky_input_t *input);
krapivsky_model_t *krapivsky_bstreap_simulate_lsu(krapivsky_input_t *input);
krapivsky_model_t *krapivsky_bstreap_simulate_lsn(krapivsky_input_t *input);
krapivsky_model_t *krapivsky_heap_simulate(krapivsky_input_t *input);
krapivsky_model_t *krapivsky_bstreap_simulate_pareto_lnu(krapivsky_input_t *input);
krapivsky_model_t *krapivsky_bstreap_simulate_pareto_lnn(krapivsky_input_t *input);
krapivsky_model_t *krapivsky_bstreap_simulate_pareto_lsu(krapivsky_input_t *input);
krapivsky_model_t *krapivsky_bstreap_simulate_pareto_lsn(krapivsky_input_t *input);
krapivsky_model_t *krapivsky_heap_simulate_pareto(krapivsky_input_t *input);
krapivsky_model_t *krapivsky_heap_simulate_quadratic(krapivsky_input_t *input);

// macro-defined
MAKE_KRAPIV_PREF_HEADERS(alpha_100)
MAKE_KRAPIV_PREF_HEADERS(alpha_101)
MAKE_KRAPIV_PREF_HEADERS(alpha_102)
MAKE_KRAPIV_PREF_HEADERS(alpha_103)
MAKE_KRAPIV_PREF_HEADERS(alpha_104)
MAKE_KRAPIV_PREF_HEADERS(alpha_105)
MAKE_KRAPIV_PREF_HEADERS(alpha_110)
MAKE_KRAPIV_PREF_HEADERS(alpha_115)
MAKE_KRAPIV_PREF_HEADERS(alpha_120)
MAKE_KRAPIV_PREF_HEADERS(alpha_140)
MAKE_KRAPIV_PREF_HEADERS(alpha_160)
MAKE_KRAPIV_PREF_HEADERS(alpha_180)
MAKE_KRAPIV_PREF_HEADERS(alpha_200)
#endif /* QUICKNET_KRAPIVSKY_H */
