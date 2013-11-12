#ifndef QUICKNET_QUICKMATH_H
#define QUICKNET_QUICKMATH_H

double mean(double *a, uint64_t n);
double stdev(double *a, uint64_t n);
unsigned int rand_init(char *base_dir, char *seed_name);

#endif /* QUICKNET_QUICKMATH_H */