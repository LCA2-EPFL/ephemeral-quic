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
