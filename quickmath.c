#include <stdint.h>
#include <math.h>

#include "quickmath.h"

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
