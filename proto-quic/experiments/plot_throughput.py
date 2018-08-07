# Plot throughtput of the TCP application in the experiment with TCP competing flow

import matplotlib.pyplot as plt
import numpy as np
import sys

def plot(filename):
    data = np.loadtxt(filename, delimiter=' ')
    timestamp = data[:, 0]
    throughput = data[:, 1]
    plt.plot(timestamp, throughput)
    plt.title("Throughput")
    plt.xlabel("time(second)")
    plt.ylabel("throughput(kbps)")
    plt.savefig("figure-tcp-throughput")
    plt.show()

plot(sys.argv[1])
