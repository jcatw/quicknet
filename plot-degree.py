#!/usr/bin/env python
# args
#  1: input node csv
#  2: input edge csv
#  3: output plot filename
#  4: p
#  5: lambda
#  6: mu

import sys
import numpy as np
import matplotlib.pyplot as plt
import statsmodels.api as sm
import os.path
from itertools import groupby

### functions
def plottable_ccdf(x,y):
    """
    Given independent variable x and unnormalized pmf y, return a plottable log-log ccdf.
    """
    fx = np.log10(x[np.logical_and(x >= tail_begins,y >= minimum_count)][:-1])
    fy = np.log10((1. - (y.cumsum() / y.cumsum()[-1]))[np.logical_and(x >= tail_begins,y >= minimum_count)][:-1])

    return fx, fy

def edgelist_to_edgedict(edgelist):
    """
    Given an edgelist, return a dictionary with edges as keys and number of duplicate edges as values.
    """
    edgedict = {}
    for edge in edgelist:
        edgedict[edge] = edgedict.get(edge,0) + 1
    return edgedict

def edgelist_to_degreedist(edgelist):
    """
    Given an edgelist, return the unnormalized pmf of out- and in-degree in the form of a {degree: mass} dictionary.
    """
    #print edgelist[0]
    indeg = {}
    outdeg = {}
    
    for (efrom,eto) in edgelist:
        indeg[eto] = indeg.get(eto,0) + 1
        outdeg[efrom] = outdeg.get(efrom,0) + 1
    
    indist = {}
    outdist = {}
    
    for efrom in outdeg.keys():
        outdist[outdeg[efrom]] = outdist.get(outdeg[efrom],0) + 1
    for eto in indeg.keys():
        indist[indeg[eto]] = indist.get(indeg[eto],0) + 1

    return outdist, indist

def degree_dict_to_arrays(degree_dict):
    """
    Given a degree dict, return arrays of degree and mass.
    """
    x = degree_dict.keys()
    x.sort()

    y = []
    for an_x in x:
        y.append(degree_dict[an_x])

    x = np.array(x).astype(float)
        
    y = np.array(y).astype(float)
    

    return x,y

def edgelist_to_plottable_ccdf(edgelist):
    """
    Given an edgelist, convert to the plottable log-log ccdf of out- and in-degree.
    """
    outdist, indist = edgelist_to_degreedist(edgelist)
    
    x_out, y_out = degree_dict_to_arrays(outdist)
    x_in , y_in  = degree_dict_to_arrays(indist)

    plottable_x_out, plottable_y_out = plottable_ccdf(x_out, y_out)
    plottable_x_in , plottable_y_in  = plottable_ccdf(x_in , y_in )

    return plottable_x_out, plottable_y_out, plottable_x_in, plottable_y_in
    
### script
tail_begins = 1    # where to begin plotting
minimum_count = 1  # minimum count of degrees considered

input_file_name = sys.argv[1]
input_edge_file_name = sys.argv[2]
plot_name = sys.argv[3]

p = float(sys.argv[4])
lamb = float(sys.argv[5])
mu = float(sys.argv[6])

v_in = 2. + p * lamb
v_out = 1. + (1. / (1. - p)) + ((mu * p) / (1. - p))

edgelist = []

edgefile = open(input_edge_file_name,"r")

for line in edgefile:
    edgestrings = line.strip().split(",")
    edgelist.append( (int(edgestrings[0]), int(edgestrings[1])) )

#print edgelist[0]

# 'reciprocated-at-all' edges
edgeset = set(edgelist)

reciprocated_edges   = []
unreciprocated_edges = []

for efrom, eto in edgelist:
    if (eto, efrom) in edgeset:
        reciprocated_edges.append((eto,efrom))
    else:
        unreciprocated_edges.append((eto,efrom))

# pairwise-reciprocated edges
edgedict = edgelist_to_edgedict(edgelist)

pairwise_reciprocated_edges = []
pairwise_unreciprocated_edges = []

for (efrom, eto) in edgeset:
    num_pairwise_reciprocated = min(edgedict.get((efrom, eto),0),
                                    edgedict.get((eto, efrom),0))
    for i in xrange(num_pairwise_reciprocated):
        pairwise_reciprocated_edges.append((efrom,eto))
    for i in xrange(num_pairwise_reciprocated, edgedict[(efrom,eto)]):
        pairwise_unreciprocated_edges.append((efrom,eto))

# before defining full-pipe function         
#total_outdist, total_indist = edgelist_to_degreedist(edgelist)
#reciprocated_outdist, reciprocated_indist = edgelist_to_degreedist(reciprocated_edges)
#unreciprocated_outdist, unreciprocated_indist = edgelist_to_degreedist(unreciprocated_edges)
#
#total_x_in, total_y_in = degree_dict_to_arrays(total_indist)
#total_x_in, total_y_in = plottable_ccdf(total_x_in, total_y_in)
#total_x_out, total_y_out = degree_dict_to_arrays(total_outdist)
#total_x_out, total_y_out = plottable_ccdf(total_x_out, total_y_out)
#    
#recip_x_in, recip_y_in = degree_dict_to_arrays(reciprocated_indist)
#recip_x_in, recip_y_in = plottable_ccdf(recip_x_in, recip_y_in)
#recip_x_out, recip_y_out = degree_dict_to_arrays(reciprocated_outdist)
#recip_x_out, recip_y_out = plottable_ccdf(recip_x_out, recip_y_out)
#
#unrecip_x_in,  unrecip_y_in =  degree_dict_to_arrays(unreciprocated_indist)
#unrecip_x_in,  unrecip_y_in = plottable_ccdf(unrecip_x_in,  unrecip_y_in)
#unrecip_x_out, unrecip_y_out = degree_dict_to_arrays(unreciprocated_outdist)
#unrecip_x_out, unrecip_y_out = plottable_ccdf(unrecip_x_out, unrecip_y_out)

#after defining full-pipe function
total_x_out, total_y_out, total_x_in, total_y_in = edgelist_to_plottable_ccdf(edgelist)
recip_x_out, recip_y_out, recip_x_in, recip_y_in = edgelist_to_plottable_ccdf(reciprocated_edges)
unrecip_x_out, unrecip_y_out, unrecip_x_in, unrecip_y_in = edgelist_to_plottable_ccdf(unreciprocated_edges)
pairwise_recip_x_out, pairwise_recip_y_out, pairwise_recip_x_in, pairwise_recip_y_in = edgelist_to_plottable_ccdf(pairwise_reciprocated_edges)
pairwise_unrecip_x_out, pairwise_unrecip_y_out, pairwise_unrecip_x_in, pairwise_unrecip_y_in = edgelist_to_plottable_ccdf(pairwise_unreciprocated_edges)



l_out = sm.OLS(total_y_out, sm.add_constant(total_x_out, prepend=False)).fit().params
l_in = sm.OLS(total_y_in, sm.add_constant(total_x_in, prepend=False)).fit().params

plt.plot(total_x_out, l_out[0] * total_x_out + l_out[1], '-', color='LightBlue')
plt.plot(total_x_in, l_in[0] * total_x_in + l_in[1], '-', color='LightGreen')

### plot recip, unrecip without all
plt.figure()
plt.title("Pairwise Reciprocated Degree Distributions of the Krapivsky Model")
plt.xlabel("Log Degree")
plt.ylabel("Log CCDF")

plt.plot(pairwise_recip_x_out,
         pairwise_recip_y_out,
         'cx',
         pairwise_recip_x_in,
         pairwise_recip_y_in,
         'rx',
         pairwise_unrecip_x_out,
         pairwise_unrecip_y_out,
         'kx',
         pairwise_unrecip_x_in,
         pairwise_unrecip_y_in,
         'mx')
         

plt.legend(('Reciprocated Out-Degree',
            'Reciprocated In-Degree', 
            'Unreciprocated Out-Degree',
            'Unreciprocated In-Degree'), 
            loc='best')


splitname = os.path.splitext(plot_name)
plotname_reciprocal = splitname[0] + '_recip' + splitname[1]
plotname_all = splitname[0] + '_all' + splitname[1]

plt.savefig(plotname_reciprocal)

### plot everything
plt.figure()
plt.title("Degree Distributions of the Krapivsky Model")
plt.xlabel("Log Degree")
plt.ylabel("Log CCDF")

plt.plot(total_x_out, 
         total_y_out, 
         'bx',
         total_x_in, 
         total_y_in, 
         'gx',
         pairwise_recip_x_out,
         pairwise_recip_y_out,
         'cx',
         pairwise_recip_x_in,
         pairwise_recip_y_in,
         'rx',
         pairwise_unrecip_x_out,
         pairwise_unrecip_y_out,
         'kx',
         pairwise_unrecip_x_in,
         pairwise_unrecip_y_in,
         'mx')
         
#plt.figtext(0.7, 0.5, 'vout: %.2f (%.2f)\nvin: %.2f (%.2f)\np: %.2f\nlambda: %.2f\nmu: %.2f' % (np.abs(l_out[0])+1., v_out, np.abs(l_in[0])+1., v_in, p, lamb, mu),bbox=dict(boxstyle='round', color='wheat', alpha=0.5))

plt.plot(total_x_out, l_out[0] * total_x_out + l_out[1], '-', color='LightBlue')
plt.plot(total_x_in, l_in[0] * total_x_in + l_in[1], '-', color='LightGreen')


plt.legend(('Out-Degree',
            'In-Degree',
            'Reciprocated Out-Degree',
            'Reciprocated In-Degree', 
            'Unreciprocated Out-Degree',
            'Unreciprocated In-Degree',
            'Out-Degree Fit', 
            'In-Degree Fit'), 
            loc='best')

plt.savefig(plotname_all)

### plot everything

plt.figure()
plt.title("Degree Distributions of the Krapivsky Model")
plt.xlabel("Log Degree")
plt.ylabel("Log CCDF")

plt.plot(total_x_out, 
         total_y_out, 
         'bx',
         total_x_in, 
         total_y_in, 
         'gx')

plt.plot(total_x_out, l_out[0] * total_x_out + l_out[1], '-', color='LightBlue')
plt.plot(total_x_in, l_in[0] * total_x_in + l_in[1], '-', color='LightGreen')

plt.legend(('Out-Degree',
            'In-Degree',
            'Out-Degree Fit',
            'In-Degree Fit'),loc="best")


plt.figtext(0.7, 0.5, 'vout: %.2f (%.2f)\nvin: %.2f (%.2f)\np: %.2f\nlambda: %.2f\nmu: %.2f' % (np.abs(l_out[0])+1., v_out, np.abs(l_in[0])+1., v_in, p, lamb, mu),bbox=dict(boxstyle='round', color='wheat', alpha=0.5))

plt.savefig(splitname[0]+splitname[1])
