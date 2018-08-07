import matplotlib.pyplot as plt
import numpy as np
import sys, os
import collections

def delay_cdf(filename):
    count = 0
    delay_count = collections.OrderedDict()
    for line in open(filename):
        line = line.strip()
        word = line.split()
        delay = float(word[1]) / 1000
        if delay in delay_count:
            delay_count[delay] = delay_count[delay] + 1
        else:
            delay_count[delay] = 1
        count += 1

    keys = sorted(delay_count.keys())
    delay_CDF = collections.OrderedDict()
    start = 0
    for key in keys:
        cur = delay_count[key]
        start += cur
        delay_CDF[key] = start / count
    return delay_CDF

def plot(filename):
    delay_CDF = delay_cdf(filename)
    lists = sorted(delay_CDF.items())
    x, y = zip(*lists)
    plt.plot(x, y)
    plt.xlabel("one way delay (ms)")
    plt.ylabel("CDF")
    plt.title("Delay CDF")
    plt.savefig("figure_delay_CDF")

plot(sys.argv[1])

