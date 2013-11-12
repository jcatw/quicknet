#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#include "quickmath.h"

#ifndef BUFSIZE
#define BUFSIZE 1024
#endif

double
mean(double *a, uint64_t n) {
  uint64_t i;
  double total = 0;
  for(i=0;i<n;i++)
    total += a[i];
  return total / (double) n;
}

double
stdev(double *a, uint64_t n) {
  uint64_t i;
  double residual;
  double sum_squared_residuals = 0.;
  double mu;

  mu = mean(a,n);
  for(i=0;i<n;i++) {
    residual = a[i] - mu;
    sum_squared_residuals += residual * residual;
  }
  
  return sqrt(sum_squared_residuals / ((double) (n - 1)));
}

unsigned int
rand_init(char *base_dir, char *seed_name) {
  unsigned int random_seed;
  // work with file descriptor for reading /dev/random
  int devrandom;
  // work with file pointer for writing seed
  FILE *seedfile; 
  char seedfilename[BUFSIZE];

  // read a random seed from /dev/random
  devrandom = open("/dev/random", O_RDONLY);
  read(devrandom, &random_seed, sizeof(random_seed));
  close(devrandom);

  // init rand
  srand(random_seed);
  
  // write seed
  sprintf(seedfilename, "%s/%s", base_dir, seed_name);
  seedfile = fopen(seedfilename, "w");
  fprintf(seedfile,"%u\n",random_seed);
  fclose(seedfile);
  
  return random_seed;
}
