#ifndef QUICKNET_BSTREAP_H
#define QUICKNET_BSTREAP_H

typedef struct bstreap bstreap_t;
typedef struct bstreap_item bstreap_item_t;

// general functions
bstreap_t *make_bstreap();
bstreap_item_t *make_bstreap_item(node_t *n, double (*compute_mass) (node_t*), uint64_t (*compute_priority) (node_t*));
void bstreap_insert(bstreap_t *bstreap,
                    node_t* node,
                    double (*compute_mass) (node_t*),
                    uint64_t (*compute_priority) (node_t*),
                    double (*compute_key)(bstreap_item_t*));
void bstreap_item_insert(bstreap_t *bstreap,
                         bstreap_item_t *item,
                         bstreap_item_t *existing_item,
                         bstreap_item_t *parent,
                         double (*compute_key)(bstreap_item_t*));
void bstreap_rotate_left(bstreap_t *bstreap, bstreap_item_t *item, bstreap_item_t *parent);
void bstreap_rotate_right(bstreap_t *bstreap, bstreap_item_t *item, bstreap_item_t *parent);
node_t *bstreap_sample(bstreap_t* bstreap);
bstreap_item_t *bstreap_item_sample(bstreap_t *bstreap,
                                    bstreap_item_t *item,
                                    bstreap_item_t *parent,
                                    double observed_mass,
                                    double uniform_sample);
void bstreap_item_splice(bstreap_t *bstreap, bstreap_item_t *item, bstreap_item_t *parent);

// abstraction providers
double get_nodemass(bstreap_item_t *item);

// curried functions (delicious)
bstreap_item_t *make_in_degree_bstreap_item_xnu(node_t *n);
bstreap_item_t *make_out_degree_bstreap_item_xnu(node_t *n);
void bstreap_in_degree_insert_lnu(bstreap_t *bstreap, node_t* node);
void bstreap_out_degree_insert_lnu(bstreap_t *bstreap, node_t* node);
void bstreap_item_insert_xnz(bstreap_t *bstreap,
                             bstreap_item_t *item,
                             bstreap_item_t *existing_item,
                             bstreap_item_t *parent);



#endif /* QUICKNET_BSTREAP_H */
