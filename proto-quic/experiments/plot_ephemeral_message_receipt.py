import matplotlib.pyplot as plt
import numpy as np
import sys

def plot(filename):
    data = np.loadtxt(filename, delimiter=',')
    xx = data[:, 0]
    timestamps = [num/1000 for num in xx]
    stream_ids = data[:, 1]
    plt.plot(timestamps, stream_ids, "ro")
    plt.title("ephemeral message receipt")
    plt.xlabel("time(ms)")
    plt.ylabel("id of the message received")
    plt.savefig("figure_ephemeral_message_receipt")
    plt.show()

plot(sys.argv[1])

