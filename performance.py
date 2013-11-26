#!/usr/bin/env python
#
# arg 1: number of runs per (type, size) pair
# arg 2: base output directory (no spaces)
# arg 3: simulate?
#  0: do not simulate, just process
#  1: simulate and process

import os
import sys
import numpy as np
import matplotlib.pyplot as plt
from scipy import stats

n_runs = int(sys.argv[1])
output_dir = sys.argv[2]
simulate = int(sys.argv[3])

small_sizes = {'heap':np.array([100,1000,10000]),
               'lnu': np.array([100,1000,10000]),
               'lnn': np.array([100,1000]),
               'lsu': np.array([100,1000,10000]),
               'lsn': np.array([100,1000,10000])}

medium_sizes = {'heap':np.array([100,1000,10000,100000,1000000]),
                'lnu': np.array([100,1000,10000,100000,1000000]),
                'lnn': np.array([100,1000,10000]),
                'lsu': np.array([100,1000,10000,100000,1000000]),
                'lsn': np.array([100,1000,10000,100000,1000000])}

large_sizes = {'heap':np.array([10000,100000,1000000,10000000]),
               'lnu': np.array([10000,100000,1000000,10000000]),
               'lnn': np.array([10000,100000]),
               'lsu': np.array([10000,100000,1000000,10000000]),
               'lsn': np.array([10000,100000,1000000,10000000])}

sizes = large_sizes
    
times = {'heap': {},
         'lnu':  {},
         'lnn':  {},
         'lsu':  {},
         'lsn':  {}} 

simtypes = sizes.keys()
         
def ensure_dir(f):
    d = os.path.dirname(f)
    if not os.path.exists(d):
        os.makedirs(d)

if simulate:
    for simtype in simtypes:
        for size in sizes[simtype]:
            print "%s: %d nodes, %d runs" % (simtype, size, n_runs)
            #output_path = "%s/%s/%s/%s.csv" % (output_dir,simtype,n_runs,size)
            output_path = "%s/%s/%s/" % (output_dir,simtype,n_runs)
            timefile = "time_%s.csv" % (size,)
            randfile = "randomseed_%s.csv" % (size,)
            ensure_dir(output_path)
            runner = "./quicknet -p 0.2 -l 3.5 -m 1.8 -o %s -r %d -n %d -t %s -u %s -s %s" % (output_path, n_runs, size, simtype, timefile, randfile)
            print runner
            os.system(runner)

for simtype in simtypes:
    for size in sizes[simtype]:
        result_path = "%s/%s/%s/time_%s.csv" % (output_dir, simtype, n_runs, size)
        results = open(result_path,"r")
        times[simtype][size] = np.array([float(time.strip()) for time in results.xreadlines()])
        

fig = plt.figure()
plt.xlabel("Log(Number of Nodes)")
plt.ylabel("Log(Processing Time (s))")
plt.title("Processing Time by Data Structure")
for simtype in simtypes:
    y = np.array([np.log10(times[simtype][size]).mean() for size in sizes[simtype]])
    y_err = np.array([2.*stats.sem(np.log10(times[simtype][size])) for size in sizes[simtype]])
    x = np.log10(sizes[simtype])
    plt.errorbar(x, y, y_err)
plt.legend(simtypes, loc="lower right")
plt.savefig(output_dir + "/performance.png")
    
