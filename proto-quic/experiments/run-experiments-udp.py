# base
mn --clean; ./clean.sh; timeout 4000 python network-udp-rtt5-buffer100-bw1-loss10.py; pkill xterm; mn --clean; ./nonshow_udp_save_result.sh udp-rtt5-buffer100-bw1-loss10

# buffer
mn --clean; ./clean.sh; timeout 4000 python network-udp-rtt5-buffer10-bw1-loss10.py; pkill xterm; mn --clean; ./nonshow_udp_save_result.sh udp-rtt5-buffer10-bw1-loss10

# loss
mn --clean; ./clean.sh; timeout 4000 python network-udp-rtt5-buffer100-bw1-loss1.py; pkill xterm; mn --clean; ./nonshow_udp_save_result.sh udp-rtt5-buffer100-bw1-loss1

# RTT
mn --clean; ./clean.sh; timeout 4000 python network-udp-rtt10-buffer100-bw1-loss10.py; pkill xterm; mn --clean; ./nonshow_udp_save_result.sh udp-rtt10-buffer100-bw1-loss10
mn --clean; ./clean.sh; timeout 4000 python network-udp-rtt20-buffer100-bw1-loss10.py; pkill xterm; mn --clean; ./nonshow_udp_save_result.sh udp-rtt20-buffer100-bw1-loss10

# bw
mn --clean; ./clean.sh; timeout 4000 python network-udp-rtt5-buffer100-bw0.1-loss10.py; pkill xterm; mn --clean; ./nonshow_udp_save_result.sh udp-rtt5-buffer100-bw0.1-loss10
