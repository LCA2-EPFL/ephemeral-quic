
import matplotlib.pyplot as plt
import numpy as np
import sys

def plot(filename):
    data = np.loadtxt(filename)
    x = data[:, 0]
    y = data[:, 1]
    plt.plot(x, y)
    plt.title("Instantaneous queue length at ri-eth1")
    plt.xlabel("time (s)")
    plt.ylabel("queue length (number of packet)")
    plt.savefig("figure_queue_length_ri")


plot(sys.argv[1])
