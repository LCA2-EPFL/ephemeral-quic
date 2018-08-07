## Run experiments

To run and collect results of
 1) single flow experiments of Ephemeral QUIC, Baseline QUIC, UDP, TCP
 2) experiments with TCP competing flow

Run the following script with root permissions:
```
# cd /home/lca2/Desktop/rolle-network/proto-quic/experiments/
# sudo ./run-experiments.sh
```

To plot the figure comparing the CDF of delays of valid messages of TCP, UDP, Baseline QUIC, Ephemeral QUIC, use the script plot_tcp_udp_baseline_ephemeral.py. For example, 
```
 python3 plot_tcp_udp_baseline_ephemeral.py aggregated/base-tcp-delay.txt aggregated/base-udp-delay.txt aggregated/base-baseline-delay.txt aggregated/base-ephemeral-delay.txt
 ```

 where the *-delay.txt files are aggregated from the multiple runs of an experiment.
 
 

