#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "clparse.h"

void clparse(clopts_t *options, int argc, char **argv) {
  //clopts_t *options;
  char c;

  //options = (clopts_t*) malloc(sizeof(*options));
  while((c = getopt(argc, argv, "n:p:l:m:e:r:")) != -1) {
    switch(c) {
    case 'n':
      sscanf(optarg, "%llu", &(options->target_n_nodes));
      break;
    case 'p':
      sscanf(optarg, "%lf", &(options->p));
      break;
    case 'l':
      sscanf(optarg, "%lf", &(options->lambda));
      break;
    case 'm':
      sscanf(optarg, "%lf", &(options->mu));
      break;
    case 'r':
      sscanf(optarg, "%lf", &(options->r));
      break;
    case 'e':
      strcpy(options->edge_file_name, optarg);
      break;
    }
  }
}
