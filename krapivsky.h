#ifndef QUICKNET_KRAPIVSKY_H
#define QUICKNET_KRAPIVSKY_H

typedef struct krapivsky_model krapivsky_model_t;
typedef union degree_set degree_set_t;

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
void make_heap_item_seed(krapivsky_model_t *km);
int8_t krapivsky_done(krapivsky_model_t *km, uint64_t target_n_nodes);
void krapivsky_next(krapivsky_model_t *km,
                    void (*node_adder) (krapivsky_model_t *km, node_t *new_node),
                    node_t* (*in_degree_sampler) (krapivsky_model_t *km),
                    node_t* (*out_degree_sampler) (krapivsky_model_t *km),
                    void (*new_node_in_degree_indexer) (krapivsky_model_t *km, node_t *node),
                    void (*new_node_out_degree_indexer) (krapivsky_model_t *km, node_t *node),
                    void (*existing_node_in_degree_indexer) (krapivsky_model_t *km, node_t *node),
                    void (*existing_node_out_degree_indexer) (krapivsky_model_t *km, node_t *node));
krapivsky_model_t* krapivsky_simulate(double p,
                                      double lambda,
                                      double mu,
                                      uint64_t target_n_nodes,
                                      void  (*set_maker)(krapivsky_model_t*),
                                      void  (*seed_maker)(krapivsky_model_t*),
                                      double (*fitness_function)(double fitness),
                                      uint8_t index_type,
                                      void (*node_adder) (krapivsky_model_t *km, node_t *new_node),
                                      node_t* (*in_degree_sampler) (krapivsky_model_t *km),
                                      node_t* (*out_degree_sampler) (krapivsky_model_t *km),
                                      void (*new_node_in_degree_indexer) (krapivsky_model_t *km, node_t *node),
                                      void (*new_node_out_degree_indexer) (krapivsky_model_t *km, node_t *node),
                                      void (*existing_node_in_degree_indexer) (krapivsky_model_t *km, node_t *node),
                                      void (*existing_node_out_degree_indexer) (krapivsky_model_t *km, node_t *node));

void krapivsky_write_edges(krapivsky_model_t *km, char *filename);
void krapivsky_free(krapivsky_model_t *km);

// abstraction providers
void make_krapivsky_bstreaps(krapivsky_model_t *km);
void make_krapivsky_heaps(krapivsky_model_t *km);
node_t *krapivsky_bstreap_in_degree_sampler(krapivsky_model_t *km);
node_t *krapivsky_bstreap_out_degree_sampler(krapivsky_model_t *km);
node_t *krapivsky_heap_in_degree_sampler(krapivsky_model_t *km);
node_t *krapivsky_heap_out_degree_sampler(krapivsky_model_t *km);
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
void krapivsky_bstreap_node_adder_lnu(krapivsky_model_t *km, node_t *new_node);
void krapivsky_bstreap_node_adder_lnn(krapivsky_model_t *km, node_t *new_node);
void krapivsky_bstreap_node_adder_lsu(krapivsky_model_t *km, node_t *new_node);
void krapivsky_bstreap_node_adder_lsn(krapivsky_model_t *km, node_t *new_node);
void krapivsky_heap_node_adder(krapivsky_model_t *km, node_t *new_node);


// curried functions
//krapivsky_model_t *make_bstreap_krapivsky_model(double p,
//                                                double lambda,
//                                                double mu);
krapivsky_model_t *krapivsky_bstreap_simulate_lnu(double p,
                                                  double lambda,
                                                  double mu,
                                                  uint64_t target_n_nodes);
krapivsky_model_t *krapivsky_bstreap_simulate_lnn(double p,
                                                  double lambda,
                                                  double mu,
                                                  uint64_t target_n_nodes);
krapivsky_model_t *krapivsky_bstreap_simulate_lsu(double p,
                                                  double lambda,
                                                  double mu,
                                                  uint64_t target_n_nodes);
krapivsky_model_t *krapivsky_bstreap_simulate_lsn(double p,
                                                  double lambda,
                                                  double mu,
                                                  uint64_t target_n_nodes);
krapivsky_model_t *krapivsky_heap_simulate(double p,
                                           double lambda,
                                           double mu,
                                           uint64_t target_n_nodes);
krapivsky_model_t *krapivsky_bstreap_simulate_pareto_lnu(double p,
                                                  double lambda,
                                                  double mu,
                                                  uint64_t target_n_nodes);
krapivsky_model_t *krapivsky_bstreap_simulate_pareto_lnn(double p,
                                                  double lambda,
                                                  double mu,
                                                  uint64_t target_n_nodes);
krapivsky_model_t *krapivsky_bstreap_simulate_pareto_lsu(double p,
                                                  double lambda,
                                                  double mu,
                                                  uint64_t target_n_nodes);
krapivsky_model_t *krapivsky_bstreap_simulate_pareto_lsn(double p,
                                                  double lambda,
                                                  double mu,
                                                  uint64_t target_n_nodes);
krapivsky_model_t *krapivsky_heap_simulate_pareto(double p,
                                           double lambda,
                                           double mu,
                                           uint64_t target_n_nodes);
#endif /* QUICKNET_KRAPIVSKY_H */
