#ifndef QUICKNET_KRAPIVSKY_H
#define QUICKNET_KRAPIVSKY_H

typedef krapivsky_model krapivsky_model_t;

// general functions
krapivsky_model_t *
make_krapivsky_model(double p,
                     double lambda,
                     double mu,
                     void  (*set_maker)(krapivsky_model_t*),
                     void  (*seed_maker)(krapivsky_model_t*));
void make_bstreap_item_seed(krapivsky_model_t *km) ;
int8_t krapivsky_done(krapivsky_model_t *km, uint64_t target_n_nodes);
void krapivsky_next(krapivsky_model_t *km,
                    node_t* (*node_adder) (krapivsky_model_t *km),
                    node_t* (*in_degree_sampler) (krapivsky_model_t *km),
                    node_t* (*out_degree_sampler) (krapivsky_model_t *km),
                    void (*in_degree_indexer) (krapivsky_model_t *km, node_t *node),
                    void (*out_degree_indexer) (krapivsky_model_t *km, node_t *node));
krapivsky_model_t* krapivsky_simulate(double p,
                                      double lambda,
                                      double mu,
                                      uint64_t target_n_nodes,
                                      void  (*set_maker)(krapivsky_model_t*),
                                      void  (*seed_maker)(krapivsky_model_t*),
                                      node_t* (*node_adder) (krapivsky_model_t *km),
                                      node_t* (*in_degree_sampler) (krapivsky_model_t *km),
                                      node_t* (*out_degree_sampler) (krapivsky_model_t *km),
                                      void (*in_degree_indexer) (krapivsky_model_t *km, node_t *node),
                                      void (*out_degree_indexer) (krapivsky_model_t *km, node_t *node));

// abstraction providers
void make_krapivsky_bstreaps(krapivsky_model_t *km);
krapivsky_model_t *krapivsky_bstreap_simulate_lnu(double p,
                                                  double lambda,
                                                  double mu,
                                                  uint64_t target_n_nodes);
node_t *krapivsky_bstreap_in_degree_sampler(krapivsky_model_t *km);
node_t *krapivsky_bstreap_out_degree_sampler(krapivsky_model_t *km);
node_t *krapivsky_bstreap_node_adder_lnu(krapivsky_model_t km);


// curried functions
//krapivsky_model_t *make_bstreap_krapivsky_model(double p,
//                                                double lambda,
//                                                double mu);
krapivsky_model_t *krapivsky_bstreap_simulate_lnu(double p,
                                                  double lambda,
                                                  double mu,
                                                  uint64_t target_n_nodes);
#endif /* QUICKNET_KRAPIVSKY_H */
