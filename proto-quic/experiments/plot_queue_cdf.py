import matplotlib.pyplot as plt
import numpy as np
import sys, os
import collections

def compute_cdf(filename):
    data = np.loadtxt(filename)
    values = data[:, 1]
    value_count = collections.OrderedDict()
    for x in values:
        if x in value_count:
            value_count[x] = value_count[x] + 1
        else:
            value_count[x] = 1
    count = len(values)
    keys = sorted(value_count.keys())
    CDF = collections.OrderedDict()
    start = 0
    for key in keys:
        cur = value_count[key]
        start += cur
        CDF[key] = start / count
    return CDF

def plot(filename):
    CDF = compute_cdf(filename)
    lists = sorted(CDF.items())
    x, y = zip(*lists)
    plt.plot(x, y)
    plt.show()

plot(sys.argv[1])
