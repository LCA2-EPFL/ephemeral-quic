# Ephemeral QUIC

An extension to QUIC optimized for ephemeral data

## Directory structure

**src/**:   source code of Ephemeral QUIC
**experiments/**:  evaluation scripts and results
**thesis/**: thesis in LaTeX

## Build instructions

This directory contains the files we used in the project Ephemeral QUIC.

You may find in /home/lca2/Desktop/rolle-network/proto-quic/src/out/Default binaries compiled:
* baseline_quic_client
* baseline_quic_server
* ephemeral_quic_client
* ephemeral_quic_server
* quic_client: corresponds to ephemeral_quic_client
* quic_server: corresponds to ephemeral_quic_server

To compile from code, run 
```
$ cd home/lca2/Desktop/rolle-network/proto-quic/src/
$ ninja -C out/Default quic_client quic_server
```
then you will find the compile binaries quic_client and quic_server also in the directory /home/lca2/Desktop/rolle-network/proto-quic/src/out/Default. 

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
 
 
 
