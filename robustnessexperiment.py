#!/usr/bin/env python
# 
# arg 1: p
# arg 2: lambda
# arg 3: mu
# arg 4: target_n_nodes
# arg 5: output dir
# arg 6: simulate networks?
import sys
import os
import matplotlib.pyplot as plt

alphas = [1.0, 1.01, 1.02, 1.03, 1.04, 1.05, 1.06, 1.07, 1.08, 1.09, 
          1.10, 1.15, 
          1.2, 
          1.3,
          1.4,
          1.5,
          1.6,
          1.7,
          1.8,
          1.9,
          2.0]

p = float(sys.argv[1])
lamb = float(sys.argv[2])
mu = float(sys.argv[3])
target_n_nodes = int(sys.argv[4])
outdir = sys.argv[5]
simulate = int(sys.argv[6])

def ensure_dir(f):
    d = os.path.dirname(f)
    if not os.path.exists(d):
        os.makedirs(d)

if simulate:
    for alpha in alphas:
        this_outdir = "%s/%s/" % (outdir, alpha)
        ensure_dir(this_outdir)
        run_string = "./quicknet -r 1 -d degrees.csv -e edges.csv -s seed.csv -o %s -p %s -l %s -m %s -n %s -t heapalpha%d" % (this_outdir,
                                                                                                                               p,
                                                                                                                               lamb,
                                                                                                                               mu,
                                                                                                                               target_n_nodes,
                                                                                                                               int(round(alpha*100.)))
        print run_string
        os.system(run_string)


max_in_degrees = []
max_out_degrees = []
num_edges = []

for alpha in alphas:
    this_outdir = "%s/%s/" % (outdir, alpha)
    n_edges = float(sum(1 for line in open("%s/edges.csv_0" % (this_outdir,))))  # count the edges

    max_in_degree = 0
    max_out_degree = 0

    for line in open("%s/degrees.csv_0" % (this_outdir,)):
        degree = line.strip().split(",")
        out_degree = float(degree[0])
        in_degree = float(degree[1])

        if out_degree > max_out_degree: 
            max_out_degree = out_degree
        if in_degree > max_in_degree: 
            max_in_degree = in_degree

    max_in_degrees.append(max_in_degree)
    max_out_degrees.append(max_out_degree)
    num_edges.append(n_edges)

out_ratios = [z[0] / z[1] for z in zip(max_out_degrees, num_edges)]
in_ratios = [z[0] / z[1] for z in zip(max_in_degrees, num_edges)]

plt.figure()
plt.title("Robustness of the Krapivsky Model")
plt.xlabel("alpha")
plt.ylabel("dmax / |E|")
plt.plot(alphas, out_ratios, alphas, in_ratios)
plt.savefig("%s/robustness.png" % (outdir,))
