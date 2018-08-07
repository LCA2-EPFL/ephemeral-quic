
import matplotlib.pyplot as plt
import numpy as np
import sys

def plot(filename):
    data = np.loadtxt(filename)
    timestamp = data[:, 0]
    packets_sent = data[:, 1]
    packets_dropped = data[:, 2]
    plt.plot(timestamp, packets_sent, label="packets sent")
    plt.plot(timestamp, packets_dropped, label="packets dropped")
    plt.xlim(xmin=0)
    plt.ylim(ymin=0)
    plt.title("Packets sent and dropped at rh-eth1")
    plt.xlabel("time (s)")
    plt.ylabel("number of packets")
    plt.legend()
    plt.savefig("figure_packets")


plot(sys.argv[1])
