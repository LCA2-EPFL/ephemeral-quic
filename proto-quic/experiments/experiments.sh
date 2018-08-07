
"""
baseline-rtt5-buffer100-bw1-loss1
ephemeral-rtt5-buffer100-bw1-loss1
ephemeral-rtt5-buffer10-bw1-loss10
baseline-rtt5-buffer100-bw1-loss10
baseline-rtt10-buffer100-bw1-loss10
baseline-rtt20-buffer100-bw1-loss10
baseline-rtt5-buffer10-bw1-loss10
"""



mn --clean; ./clean.sh
nohup python network-baseline-rtt5-buffer100-bw1-loss1.py
sleep 30
#pid=$(pgrep baseline_quic_client)
pid=$(ps -ef | grep baseline_quic_client | grep -v xterm | grep -v grep | awk '{print $2}')
echo $pid
timeout 6000 tail --pid=$pid -f /dev/null
sleep 30
pkill xterm; mn --clean; ./nonshow_save_result.sh baseline-rtt5-buffer100-bw1-loss1

mn --clean; ./clean.sh
nohup python network-ephemeral-rtt5-buffer100-bw1-loss1.py
sleep 30
pid=$(pgrep quic_client)
echo $pid
timeout 6000 tail --pid=$pid -f /dev/null
sleep 30
pkill xterm; mn --clean; ./nonshow_save_result.sh ephemeral-rtt5-buffer100-bw1-loss1


mn --clean; ./clean.sh
nohup python network-ephemeral-rtt5-buffer10-bw1-loss10.py
sleep 30
pid=$(pgrep quic_client)
echo $pid
timeout 6000 tail --pid=$pid -f /dev/null
sleep 30
pkill xterm; mn --clean; ./nonshow_save_result.sh ephemeral-rtt5-buffer10-bw1-loss10


mn --clean; ./clean.sh
nohup python network-baseline-rtt5-buffer100-bw1-loss10.py
sleep 30
pid=$(ps -ef | grep baseline_quic_client  | grep -v xterm | grep -v grep |  awk '{print $2}')
echo $pid
timeout 6000 tail --pid=$pid -f /dev/null
sleep 30
pkill xterm; mn --clean; ./nonshow_save_result.sh baseline-rtt5-buffer100-bw1-loss10



mn --clean; ./clean.sh
nohup python network-baseline-rtt10-buffer100-bw1-loss10.py
sleep 30
pid=$(ps -ef | grep baseline_quic_client  | grep -v xterm | grep -v grep | awk '{print $2}')
echo $pid
timeout 6000 tail --pid=$pid -f /dev/null
sleep 30
pkill xterm; mn --clean; ./nonshow_save_result.sh baseline-rtt10-buffer100-bw1-loss10


mn --clean; ./clean.sh
nohup python network-baseline-rtt20-buffer100-bw1-loss10.py
sleep 30
pid=$(ps -ef | grep baseline_quic_client  | grep -v xterm | grep -v grep |  awk '{print $2}')
echo $pid
timeout 6000 tail --pid=$pid -f /dev/null
sleep 30
pkill xterm; mn --clean; ./nonshow_save_result.sh baseline-rtt20-buffer100-bw1-loss10



mn --clean; ./clean.sh
nohup python network-baseline-rtt5-buffer10-bw1-loss10.py
sleep 30
pid=$(ps -ef | grep baseline_quic_client  | grep -v xterm | grep -v grep | awk '{print $2}')
echo $pid
timeout 6000 tail --pid=$pid -f /dev/null
sleep 30
pkill xterm; mn --clean; ./nonshow_save_result.sh baseline-rtt5-buffer10-bw1-loss10


