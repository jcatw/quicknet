#!/usr/bin/env python
# 
# arg 1: p
# arg 2: lambda
# arg 3: mu
# arg 4: target_n_nodes
# arg 5: type
# arg 6: output dir
import sys
import os

p = float(sys.argv[1])
lamb = float(sys.argv[2])
mu = float(sys.argv[3])
target_n_nodes = int(sys.argv[4])
simtype = sys.argv[5]
outdir = sys.argv[6]

def ensure_dir(f):
    d = os.path.dirname(f)
    if not os.path.exists(d):
        os.makedirs(d)

ensure_dir("%s/edges.csv_0" % (outdir,))
run_string = "./quicknet -r 1 -e edges.csv -s seed.csv -o %s -p %s -l %s -m %s -n %s -t %s" % (outdir,
                                                                                               p,
                                                                                               lamb,
                                                                                               mu,
                                                                                               target_n_nodes,
                                                                                               simtype)
print run_string
os.system(run_string)

plot_string = "./plot-degree.py dummy %s/edges.csv_0 %s/logccdf.png %s %s %s" % (outdir,
                                                                                 outdir,
                                                                                 p,
                                                                                 lamb,
                                                                                 mu)
print plot_string
os.system(plot_string)
