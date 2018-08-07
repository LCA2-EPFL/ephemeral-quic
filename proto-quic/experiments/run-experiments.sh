# Scripts to run
# 1) single flow experiments of Ephemeral QUIC, Baseline QUIC, UDP, TCP
# 2) experiments with TCP competing flow

########################### Ephemeral QUIC ################################

# base
mn --clean; ./clean.sh
nohup python network-ephemeral-rtt5-buffer100-bw1-loss10.py
sleep 30
pid=$(pgrep quic_client)
echo $pid
tail --pid=$pid -f /dev/null
sleep 30
pkill xterm; mn --clean; ./nonshow_save_result.sh ephemeral-rtt5-buffer100-bw1-loss10

# buffer
sleep 300
mn --clean; ./clean.sh
nohup python network-ephemeral-rtt5-buffer10-bw1-loss10.py
sleep 30
pid=$(pgrep quic_client)
echo $pid
tail --pid=$pid -f /dev/null
sleep 30
pkill xterm; mn --clean; ./nonshow_save_result.sh ephemeral-rtt5-buffer10-bw1-loss10

# loss
sleep 300
mn --clean; ./clean.sh
nohup python network-ephemeral-rtt5-buffer100-bw1-loss1.py
sleep 30
pid=$(pgrep quic_client)
echo $pid
tail --pid=$pid -f /dev/null
sleep 30
pkill xterm; mn --clean; ./nonshow_save_result.sh ephemeral-rtt5-buffer100-bw1-loss1

# RTT
sleep 300
mn --clean; ./clean.sh
nohup python network-ephemeral-rtt10-buffer100-bw1-loss10.py
sleep 30
pid=$(pgrep quic_client)
echo $pid
tail --pid=$pid -f /dev/null
sleep 30
pkill xterm; mn --clean; ./nonshow_save_result.sh ephemeral-rtt10-buffer100-bw1-loss10

sleep 300
mn --clean; ./clean.sh
nohup python network-ephemeral-rtt20-buffer100-bw1-loss10.py
sleep 30
pid=$(pgrep quic_client)
echo $pid
tail --pid=$pid -f /dev/null
sleep 30
pkill xterm; mn --clean; ./nonshow_save_result.sh ephemeral-rtt20-buffer100-bw1-loss10


# bw
sleep 300
mn --clean; ./clean.sh
nohup python network-ephemeral-rtt5-buffer100-bw0.1-loss10.py
sleep 30
pid=$(pgrep quic_client)
echo $pid
tail --pid=$pid -f /dev/null
sleep 30
pkill xterm; mn --clean; ./nonshow_save_result.sh ephemeral-rtt5-buffer100-bw0.1-loss10


#################################### Baseline QUIC #####################################

# base
sleep 300
mn --clean; ./clean.sh
nohup python network-baseline-rtt5-buffer100-bw1-loss10.py
sleep 30
pid=$(ps -ef | grep baseline_quic_client  | grep -v xterm | grep -v grep | awk '{print $2}')
echo $pid
tail --pid=$pid -f /dev/null
sleep 30
pkill xterm; mn --clean; ./nonshow_save_result.sh baseline-rtt5-buffer100-bw1-loss10

# buffer
sleep 300
mn --clean; ./clean.sh
nohup python network-baseline-rtt5-buffer10-bw1-loss10.py
sleep 30
pid=$(ps -ef | grep baseline_quic_client  | grep -v xterm | grep -v grep | awk '{print $2}')
echo $pid
tail --pid=$pid -f /dev/null
sleep 30
pkill xterm; mn --clean; ./nonshow_save_result.sh baseline-rtt5-buffer10-bw1-loss10

# loss
sleep 300
mn --clean; ./clean.sh
nohup python network-baseline-rtt5-buffer100-bw1-loss1.py
sleep 30
pid=$(ps -ef | grep baseline_quic_client  | grep -v xterm | grep -v grep | awk '{print $2}')
echo $pid
tail --pid=$pid -f /dev/null
sleep 30
pkill xterm; mn --clean; ./nonshow_save_result.sh baseline-rtt5-buffer100-bw1-loss1

# RTT
sleep 300
mn --clean; ./clean.sh
nohup python network-baseline-rtt10-buffer100-bw1-loss10.py
sleep 30
pid=$(ps -ef | grep baseline_quic_client  | grep -v xterm | grep -v grep | awk '{print $2}')
echo $pid
tail --pid=$pid -f /dev/null
sleep 30
pkill xterm; mn --clean; ./nonshow_save_result.sh baseline-rtt10-buffer100-bw1-loss10


sleep 300
mn --clean; ./clean.sh
nohup python network-baseline-rtt20-buffer100-bw1-loss10.py
sleep 30
pid=$(ps -ef | grep baseline_quic_client  | grep -v xterm | grep -v grep | awk '{print $2}')
echo $pid
tail --pid=$pid -f /dev/null
sleep 30
pkill xterm; mn --clean; ./nonshow_save_result.sh baseline-rtt20-buffer100-bw1-loss10

# bw
sleep 300
mn --clean; ./clean.sh
nohup python network-baseline-rtt5-buffer100-bw0.1-loss10.py
sleep 30
pid=$(ps -ef | grep baseline_quic_client  | grep -v xterm | grep -v grep | awk '{print $2}')
echo $pid
tail --pid=$pid -f /dev/null
sleep 30
pkill xterm; mn --clean; ./nonshow_save_result.sh baseline-rtt5-buffer100-bw0.1-loss10


################################### UDP ###############################################

# base
mn --clean; ./clean.sh; timeout 4000 python network-udp-rtt5-buffer100-bw1-loss10.py; pkill xterm; mn --clean; ./nonshow_udp_save_result.sh

# buffer
mn --clean; ./clean.sh; timeout 4000 python network-udp-rtt5-buffer10-bw1-loss10.py; pkill xterm; mn --clean; ./nonshow_udp_save_result.sh

# loss
mn --clean; ./clean.sh; timeout 4000 python network-udp-rtt5-buffer100-bw1-loss1.py; pkill xterm; mn --clean; ./nonshow_udp_save_result.sh

# RTT
mn --clean; ./clean.sh; timeout 4000 python network-udp-rtt10-buffer100-bw1-loss10.py; pkill xterm; mn --clean; ./nonshow_udp_save_result.sh
mn --clean; ./clean.sh; timeout 4000 python network-udp-rtt20-buffer100-bw1-loss10.py; pkill xterm; mn --clean; ./nonshow_udp_save_result.sh

# bw
mn --clean; ./clean.sh; timeout 4000 python network-udp-rtt5-buffer100-bw0.1-loss10.py; pkill xterm; mn --clean; ./nonshow_udp_save_result.sh


##################################### TCP #############################################


# tcp base
sleep 300
mn --clean; ./clean.sh
echo "START"
nohup python network-tcp-rtt5-buffer100-bw1-loss10.py
sleep 10
echo "haha"
pid=$(pgrep -f "python3 tcp_client.py$")
echo $pid
tail --pid=$pid -f /dev/null
sleep 30
pkill xterm; mn --clean; ./nonshow_tcp_save_result.sh



# tcp shallow buffer
sleep 300
mn --clean; ./clean.sh
echo "START"
nohup python network-tcp-rtt5-buffer10-bw1-loss10.py
sleep 10
echo "haha"
pid=$(pgrep -f "python3 tcp_client.py$")
echo $pid
tail --pid=$pid -f /dev/null
sleep 30
pkill xterm; mn --clean; ./nonshow_tcp_save_result.sh



# tcp rtt
sleep 300
mn --clean; ./clean.sh
echo "START"
nohup python network-tcp-rtt10-buffer100-bw1-loss10.py
sleep 10
echo "haha"
pid=$(pgrep -f "python3 tcp_client.py$")
echo $pid
tail --pid=$pid -f /dev/null
sleep 30
pkill xterm; mn --clean; ./nonshow_tcp_save_result.sh


sleep 300
mn --clean; ./clean.sh
echo "START"
nohup python network-tcp-rtt20-buffer100-bw1-loss10.py
sleep 10
echo "haha"
pid=$(pgrep -f "python3 tcp_client.py$")
echo $pid
tail --pid=$pid -f /dev/null
sleep 30
pkill xterm; mn --clean; ./nonshow_tcp_save_result.sh



############################### With TCP competing flow #################

# with Ephemeral QUIC
mn --clean; ./clean.sh
nohup python network-competing-ephemeral-rtt5-buffer100-bw0.1-loss1.py
sleep 30
pid=$(pgrep quic_client)
echo $pid
tail --pid=$pid -f /dev/null
sleep 30
pkill xterm; mn --clean; ./nonshow_save_result_competing_flows.sh competing-ephemeral-rtt5-buffer100-bw0.1-loss1.py

# with Baseline QUIC
mn --clean; ./clean.sh
nohup python network-competing-baseline-rtt5-buffer100-bw0.1-loss1.py
sleep 30
pid=$(ps -ef | grep baseline_quic_client  | grep -v xterm | grep -v grep | awk '{print $2}')
echo $pid
tail --pid=$pid -f /dev/null
sleep 30
pkill xterm; mn --clean; ./nonshow_save_result_competing_flows.sh competing-baseline-rtt5-buffer100-bw0.1-loss1
