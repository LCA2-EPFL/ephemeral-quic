dirname=$(date +%Y%m%d-%H%M)-udp
mkdir $dirname
mv client.pcap $dirname
mv server.pcap $dirname
mv udp-output-server.txt $dirname
mv udp-output-client.txt $dirname
mv qlen*.txt $dirname
cp network-quic.py $dirname
cp network-quic-udp.py $dirname
./handle-udp-output < $dirname/output-udp-server.txt > $dirname/statistics.txt
python3 plot_queue_length_ri_nonshow.py $dirname/qlen_ri.txt
cp /home/lca2/Desktop/rolle-network/proto-quic/src/net/tools/quic/quic_spdy_client_base.cc $dirname/
mv queue.txt $dirname
python3 plot_packets_sent_and_dropped_nonshow.py $dirname/queue.txt 
cp udp_client.py $dirname/
cp udp_server.py $dirname/
mv *.png $dirname/



