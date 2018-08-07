import matplotlib.pyplot as plt
import numpy as np
import sys

def plot_streams(filename):
    x = np.loadtxt(filename)
    plt.plot(x)
    plt.title("number of open streams")
    plt.xlabel("message id")
    plt.ylabel("number of open streams")
    plt.savefig("figure_num_open_streams")

plot_streams(sys.argv[1])

