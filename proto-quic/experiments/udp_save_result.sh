dirname=$(date +%Y%m%d-%H%M)
if [ $# -eq 1 ]
then
  dirname=dirname$1
fi
mkdir $dirname
mv client.pcap $dirname
mv server.pcap $dirname
mv udp-output-server.txt $dirname
mv udp-output-client.txt $dirname
mv qlen*.txt $dirname
./handle-udp-output < $dirname/udp-output-server.txt | tee $dirname/statistics.txt
./extract-delay-udp-output < $dirname/udp-output-server.txt > $dirname/delay.txt
python3 plot_udp_delay_cdf.py $dirname/delay.txt &
python3 plot_queue_length_ri.py $dirname/qlen_ri.txt &
mv queue.txt $dirname
python3 plot_packets_sent_and_dropped.py $dirname/queue.txt & 
cp udp_client.py $dirname/
cp udp_server.py $dirname/
sleep 5
mv *.png $dirname/


