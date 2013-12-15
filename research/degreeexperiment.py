#!/usr/bin/env python
# 
# arg 1: p
# arg 2: lambda
# arg 3: mu
# arg 4: target_n_nodes
# arg 5: type
# arg 6: output dir
# arg 7: number of runs
#
import sys
import os

p = float(sys.argv[1])
lamb = float(sys.argv[2])
mu = float(sys.argv[3])
target_n_nodes = int(sys.argv[4])
simtype = sys.argv[5]
outdir = sys.argv[6]
nruns = int(sys.argv[7])

def ensure_dir(f):
    d = os.path.dirname(f)
    if not os.path.exists(d):
        os.makedirs(d)

ensure_dir("%s/edges.csv_0" % (outdir,))
#ensure_dir("%s/degrees.csv_0" % (outdir,))
run_string = "./quicknet -r %s -e edges.csv -s seed.csv -o %s -p %s -l %s -m %s -n %s -t %s" % (nruns,
                                                                                                outdir,
                                                                                                p,
                                                                                                lamb,
                                                                                                mu,
                                                                                                target_n_nodes,
                                                                                                simtype)
print run_string
os.system(run_string)

plot_string = "./plot-degree-many.py dummy %s/edges.csv %s/logccdf.png %s %s %s %s" % (outdir,
                                                                                       outdir,
                                                                                       p,
                                                                                       lamb,
                                                                                       mu,
                                                                                       nruns)
print plot_string
os.system(plot_string)
