# base
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


