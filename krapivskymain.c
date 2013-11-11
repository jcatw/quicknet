#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include "networknode.h"
#include "bstreap.h"
#include "heap.h"
#include "krapivsky.h"
#include "quickmath.h"

#define BUFSIZE 128

int main(int argc, char **argv) {
  // counter
  uint64_t i;
  // arg placeholder for getopt
  int c;
  // timing variables
  clock_t t1, t2;

  // model data structure
  krapivsky_model_t *km;
  // model parameters
  double p = 0.2;
  double lambda = 3.5;
  double mu = 1.8;
  // number of nodes to simulate
  uint64_t target_n_nodes = 1000;
  // number of simulations to run
  uint64_t n_runs = 1;
  // base output filename
  char out_file_name[BUFSIZE] = "results/edges.csv";
  // output filename for each run, constructed automatically
  char run_file_name[BUFSIZE];
  // output filename for timing data
  char time_file_name[BUFSIZE];
  FILE *time_file;
  // write out timing information?
  int write_time = 0;
  // array of execution times
  double *times;
  // the type of data structure to use for node indexing
  char type[BUFSIZE] = "heap";
  // write out the network's edges?
  int write_edges = 0;
  

  // parse command line args
  while((c = getopt(argc, argv, "t:p:m:l:n:o:r:u:")) != -1){
    switch(c) {
    case 't':
      strcpy(type, optarg);
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
    case 'o':
      write_edges = 1;
      strcpy(out_file_name,optarg);
      break;
    case 'r':
      sscanf(optarg, "%llu", &n_runs);
      break;
    case 'u':
      write_time = 1;
      strcpy(time_file_name,optarg);
      break;
    }
  }

  // open the time file if we'll be using it
  if(write_time) {
    times = (double*) malloc(n_runs * sizeof(*times));
    time_file = fopen(time_file_name,"w");
  }
  // run the simulations
  for(i=0;i<n_runs;i++) {
    t1 = clock();
    if(strcmp(type,"lnu") == 0) {
      km = krapivsky_bstreap_simulate_lnu(p, lambda, mu, target_n_nodes);
    } else if(strcmp(type,"lnn") == 0) {
      km = krapivsky_bstreap_simulate_lnn(p, lambda, mu, target_n_nodes);
    } else if(strcmp(type,"lsu") == 0) {
      km = krapivsky_bstreap_simulate_lsu(p, lambda, mu, target_n_nodes);
    } else if(strcmp(type,"lsn") == 0) {
      km = krapivsky_bstreap_simulate_lsn(p, lambda, mu, target_n_nodes);
    } else if(strcmp(type,"heap") == 0) {
      km = krapivsky_heap_simulate(p, lambda, mu, target_n_nodes);
    } else {
      printf("Unknown type: %s\n",type);
      return 1;
    }
    t2 = clock();
    if(write_edges) {
      sprintf(run_file_name,"%s_%llu",out_file_name,i);
      krapivsky_write_edges(km,run_file_name);
    }
    if(write_time) {
      times[i] = ((double) (t2 - t1))/CLOCKS_PER_SEC;  //time in seconds
      fprintf(time_file, "%lf\n", times[i]);  
    }
    krapivsky_free(km);
  }

  // output and clean up timing data
  if(write_time) {
    printf("Simulated %llu networks with %llu nodes each.\nEach network took %lf +/- %lf seconds to simulate.\n",
           n_runs,
           target_n_nodes,
           mean(times,n_runs),
           stdev(times,n_runs));
    free(times);
    fclose(time_file);
  }
  return 0;
}
