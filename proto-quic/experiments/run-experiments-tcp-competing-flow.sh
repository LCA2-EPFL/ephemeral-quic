# with Ephemeral QUIC
mn --clean; ./clean.sh
nohup python network-competing-ephemeral-rtt5-buffer100-bw0.1-loss1.py
sleep 30
pid=$(pgrep quic_client)
echo $pid
tail --pid=$pid -f /dev/null
sleep 30
pkill xterm; mn --clean; ./nonshow_save_result_competing_flows.sh competing-ephemeral-rtt5-buffer100-bw0.1-loss1

# with Baseline QUIC
mn --clean; ./clean.sh
nohup python network-competing-baseline-rtt5-buffer100-bw0.1-loss1.py
sleep 30
pid=$(ps -ef | grep baseline_quic_client  | grep -v xterm | grep -v grep | awk '{print $2}')
echo $pid
tail --pid=$pid -f /dev/null
sleep 30
pkill xterm; mn --clean; ./nonshow_save_result_competing_flows.sh competing-baseline-rtt5-buffer100-bw0.1-loss1
