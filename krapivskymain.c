#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>


#include "networknode.h"
#include "bstreap.h"
#include "heap.h"
#include "krapivsky.h"


#define BUFSIZE 128

int main(int argc, char **argv) {
  krapivsky_model_t *km;
  uint64_t target_n_nodes;
  char out_file_name[BUFSIZE];

  target_n_nodes = atoi(argv[1]);
  
  sprintf(out_file_name,"results/%s/edges_%llu.csv","heap",target_n_nodes);
  //km = krapivsky_bstreap_simulate_lnu(0.2, 3.5, 1.8, target_n_nodes);
  //km = krapivsky_bstreap_simulate_lnn(0.2, 3.5, 1.8, target_n_nodes);
  //km = krapivsky_bstreap_simulate_lsu(0.2, 3.5, 1.8, target_n_nodes);
  //km = krapivsky_bstreap_simulate_lsn(0.2, 3.5, 1.8, target_n_nodes);
  km = krapivsky_heap_simulate(0.2, 3.5, 1.8, target_n_nodes);

  krapivsky_write_edges(km,out_file_name);
  
  return 0;
}
