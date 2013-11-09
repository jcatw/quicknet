#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>


#include "networknode.h"
#include "bstreap.h"
#include "heap.h"
#include "krapivsky.h"


#define BUFSIZE 128

int main(int argc, char **argv) {
  krapivsky_model_t *km;
  double p, lambda, mu;
  uint64_t target_n_nodes;
  char out_file_name[BUFSIZE];
  char type[BUFSIZE];
  int c;

  //target_n_nodes = atoi(argv[1]);

  while((c = getopt(argc, argv, "t:p:m:l:n:")) != -1){
    switch(c) {
    case 't':
      strcpy(out_file_name, optarg);
      break;
    case 'p':
      sscanf(optarg, "%lf", &p);
      break;
    case 'm':
      sscanf(optarg, "%lf", &mu);
      break;
    case 'l':
      sscanf(optarg, "%lf", &lambda);
      break;
    case 'n':
      sscanf(optarg, "%llu", &target_n_nodes);
      break;
    }
  }
  
  sprintf(out_file_name,"results/%s/edges_%llu.csv",type,target_n_nodes);

  if(strcmp(type,"lnu")) {
    km = krapivsky_bstreap_simulate_lnu(p, lambda, mu, target_n_nodes);
  } else if(strcmp(type,"lnn")) {
    km = krapivsky_bstreap_simulate_lnn(p, lambda, mu, target_n_nodes);
  } else if(strcmp(type,"lsu")) {
    km = krapivsky_bstreap_simulate_lsu(p, lambda, mu, target_n_nodes);
  } else if(strcmp(type,"lsn")) {
    km = krapivsky_bstreap_simulate_lsn(p, lambda, mu, target_n_nodes);
  } else if(strcmp(type,"heap")) {
    km = krapivsky_heap_simulate(p, lambda, mu, target_n_nodes);
  }
  
  krapivsky_write_edges(km,out_file_name);
  
  //km = krapivsky_bstreap_simulate_lnu(0.2, 3.5, 1.8, target_n_nodes);
  //km = krapivsky_bstreap_simulate_lnn(0.2, 3.5, 1.8, target_n_nodes);
  //km = krapivsky_bstreap_simulate_lsu(0.2, 3.5, 1.8, target_n_nodes);
  //km = krapivsky_bstreap_simulate_lsn(0.2, 3.5, 1.8, target_n_nodes);
  //km = krapivsky_heap_simulate(0.2, 3.5, 1.8, target_n_nodes);
  return 0;
}
