# Example usage: python3 plot_tcp_udp_baseline_ephemeral.py aggregated/base-tcp-delay.txt aggregated/base-udp-delay.txt aggregated/base-baseline-delay.txt aggregated/base-ephemeral-delay.txt

import matplotlib.pyplot as plt
import numpy as np
import sys
import collections

def delay_cdf(filename, count):
    count = 0
    delay_count = collections.OrderedDict()
    for line in open(filename):
        line = line.strip()
        word = line.split()
        delay = float(word[1]) / 1000
        if (delay > 100):
            continue
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
    delay_CDF[100] = start / count
    return delay_CDF

def plot_quic(filename, name, count, m, c):
    delay_CDF = delay_cdf(filename, count)
    lists = sorted(delay_CDF.items())
    x, y = zip(*lists)
    plt.plot(x, y, label=name, marker=m, markevery=1000, color=c)


def delay_cdf_tcp_or_udp(filename, count):
    count = 0
    delay_count = collections.OrderedDict()
    for line in open(filename):
        delay = float(line)
        if (delay > 100):
            continue
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
    delay_CDF[100] = start / count
    return delay_CDF

def plot_tcp_or_udp(filename, name, count, m, c):
    delay_CDF = delay_cdf_tcp_or_udp(filename, count)
    lists = sorted(delay_CDF.items())
    x, y = zip(*lists)
    plt.plot(x, y, label=name, marker=m, markevery=1000, color=c)


plot_tcp_or_udp(sys.argv[1], "TCP", 108000, '^', '#1f77b4')
plot_tcp_or_udp(sys.argv[2], "UDP", 108000, 'v', '#ff7f0e')
plot_quic(sys.argv[3], "Baseline QUIC", 108000, 'x', '#2ca02c')
plot_quic(sys.argv[4], "Ephemeral QUIC", 108000, 'o', '#d62728')
plt.xlim(xmin=0, xmax=100)
plt.ylim(ymin=0)#, ymax=1)


plt.title("CDF of delay of valid messages received")
plt.xlabel("delay (ms)")
plt.ylabel("CDF of delay normalized by all messages sent")
plt.legend()
plt.savefig("delays")
plt.show()

