#ifndef QUICKNET_QUICKMATH_H
#define QUICKNET_QUICKMATH_H

double mean(double *a, uint64_t n);
double stdev(double *a, uint64_t n);
double randn(double mu, double sigma);
unsigned int rand_init(char *base_dir, char *seed_name);
double sample_fitness_pareto_in(double lambda, double p);
double sample_fitness_pareto_out(double mu, double p);
double sample_fitness_normal_in(double lambda, double p);
double sample_fitness_normal_out(double mu, double p);
double identity(double x, double p);

#endif /* QUICKNET_QUICKMATH_H */
