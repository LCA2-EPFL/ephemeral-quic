dirname=$(date +%Y%m%d-%H%M)
if [ $# -eq 1 ]
then
  dirname=$dirname-$1
fi
mkdir $dirname
mv client.pcap $dirname
mv server.pcap $dirname
mv tcp-output-server.txt $dirname
mv tcp-output-client.txt $dirname
mv qlen*.txt $dirname
python3 plot_queue_length_ri_nonshow.py $dirname/qlen_ri.txt
cp /home/lca2/Desktop/rolle-network/proto-quic/src/net/tools/quic/quic_spdy_client_base.cc $dirname/
mv queue.txt $dirname
python3 plot_packets_sent_and_dropped_nonshow.py $dirname/queue.txt 
cp tcp_client.py $dirname/
cp tcp_server.py $dirname/
mv *.png $dirname/

