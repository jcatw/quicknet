#!/usr/bin/env python
#
# arg 1: output path
# arg 2: number of nodes
import sys
import os
import numpy as np
import matplotlib.pyplot as plt

outpath = sys.argv[1]
n_nodes = int(sys.argv[2])

def ensure_dir(f):
    d = os.path.dirname(f)
    if not os.path.exists(d):
        os.makedirs(d)

heat_file = "%s/%s" % (outpath,"heat.csv")
ensure_dir(heat_file)

run_string_template = "./quicknet -o %s -r 1 -s seed.csv -p 0.2 -l 3.5 -m 1.8 -n %s " % (outpath,n_nodes)
run_string_template += "-e edges_%s.csv -t %s"

run_string_linear = run_string_template % ("heap","heap")
run_string_variable_fitness = run_string_template % ("heappareto","heappareto")
run_string_nonlinear = run_string_template % ("heapquadratic","heapquadratic")

run_octave_string = "octave --eval \"similarityexperiment('%s/edges_heap.csv_0','%s/edges_heappareto.csv_0','%s/edges_heapquadratic.csv_0','%s/heat.csv',%d)\"" % (outpath, outpath, outpath, outpath, n_nodes)

print run_string_linear
os.system(run_string_linear)
print run_string_variable_fitness
os.system(run_string_variable_fitness)
print run_string_nonlinear
os.system(run_string_nonlinear)

print run_octave_string
os.system(run_octave_string)

#heat_data = np.genfromtxt(heat_file, dtype=float, delimiter=',', names=False)
f = open(heat_file,"r")
heat_data = np.array([[float(x) for x in line.strip().split(",")] for line in f.xreadlines()])
heat_x = np.array(range(heat_data.shape[0]))

plt.figure()
plt.ylabel("heat")
plt.xlabel("tstep")
plt.title("Transient Heat Behavior")
plt.plot(heat_x, heat_data[:,0], heat_x, heat_data[:,1], heat_x, heat_data[:,2])
plt.legend(("Linear", "Pareto Fitness", "Quadratic"))
plt.savefig(outpath+"/heat.png")
