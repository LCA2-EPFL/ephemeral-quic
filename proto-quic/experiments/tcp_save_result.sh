dirname=$(date +%Y%m%d-%H%M)-with-tcp
mkdir $dirname
mv client.pcap $dirname
mv server.pcap $dirname
mv output-client.txt $dirname
mv output-server.txt $dirname
mv qlen*.txt $dirname
mv /home/lca2/Desktop/delay_client_server.txt $dirname
mv /home/lca2/Desktop/delay_server_client.txt $dirname
mv /home/lca2/Desktop/Old_Messages.txt $dirname
cp network-quic.py $dirname
./num_open_streams < $dirname/output-client.txt > $dirname/num_open_streams.txt
python3 plot_num_open_streams.py $dirname/num_open_streams.txt &
./preprocess_message_receipt  $dirname/delay_client_server.txt < $dirname/output-server.txt > $dirname/ephemeral_message_receipt.txt
python3 plot_ephemeral_message_receipt.py $dirname/ephemeral_message_receipt.txt &
python3 plot_delay_cdf.py $dirname/delay_client_server.txt &
# python3 plot_queue_length_rh.py $dirname/qlen_rh.txt &
python3 plot_queue_length_ri.py $dirname/qlen_ri.txt &
./delay < $dirname/delay_client_server.txt | tee statistics.txt
cp /home/lca2/Desktop/rolle-network/proto-quic/src/net/tools/quic/quic_spdy_client_base.cc $dirname/
cp /home/lca2/Desktop/rolle-network/proto-quic/src/net/tools/quic/quic_simple_client_bin.cc $dirname/
mv queue.txt $dirname
python3 plot_packets_sent_and_dropped.py $dirname/queue.txt 
mv statistics.txt $dirname
mv *.png $dirname/
mv tcp-output* $dirname/



