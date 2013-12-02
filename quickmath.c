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

double
randn(double mu, double sigma) {
  // Use CLT-based approximation to normal.  If u is a vector of 12
  // standard uniform samples, then sum(u) - 6 approximates a standard
  // normal random variable.
  int i;
  double sum_uniform_samples = 0.;
  double standard_normal_sample;
  double transformed_normal_sample;

  for(i=0;i<12;i++)
    sum_uniform_samples += (rand() / (double)RAND_MAX);
  standard_normal_sample = sum_uniform_samples - 6.;
  transformed_normal_sample = mu + sigma * standard_normal_sample;

  return transformed_normal_sample;
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

double
sample_fitness_pareto_in(double lambda, double p) {
  double xm;
  xm = (lambda - 1.);
  //xm = ((1. + p * lambda) / (2. + p * lambda)) * lambda;
  return xm / pow((((double) rand()) / RAND_MAX),1./lambda);
}

double
sample_fitness_pareto_out(double mu, double p) {
  double xm;
  //double qm1 = (1. / (1. - p));
  xm = (mu - 1.);
  //xm = ((qm1 + mu * p * qm1) / (1 + qm1 + mu * p * qm1)) * mu;
  //xm = ( (1. / (1. - p + p * lambda) / (2. + p * lambda)) * lambda;
  return xm / pow((((double) rand()) / RAND_MAX),1./mu);
}

double
sample_fitness_normal_in(double lambda, double p) {
  double sampled_fitness;
  // if sigma = fitness / 4., then chance of a negative value is less
  // than 1 in 10,000
  sampled_fitness = randn(lambda, lambda/4.);
  // truncate negative values
  if(sampled_fitness < 0.)
    sampled_fitness = 0.;
  return sampled_fitness;
}

double
sample_fitness_normal_out(double mu, double p) {
  double sampled_fitness;
  // if sigma = fitness / 4., then chance of a negative value is less
  // than 1 in 10,000
  sampled_fitness = randn(mu, mu/4.);
  // truncate negative values
  if(sampled_fitness < 0.)
    sampled_fitness = 0.;
  return sampled_fitness;
}

double
identity(double x, double p) {
  return x;
}
