#ifndef QUICKNET_CLPARSE_H
#define QUICKNET_CLPARSE_H

#define BUFFER_MAX 1024

typedef struct clopts clopts_t;

struct clopts {
  uint64_t target_n_nodes;
  double p;
  double lambda;
  double mu;
  char edge_file_name[BUFFER_MAX];
};

void clparse(clopts_t *options, int argc, char **argv);

#endif /* QUICKNET_CLPARSE_H */
