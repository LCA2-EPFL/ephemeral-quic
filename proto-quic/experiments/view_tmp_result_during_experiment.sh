dirname=tmp
rm $dirname -rf
mkdir $dirname
cp client.pcap $dirname
cp server.pcap $dirname
cp output-client.txt $dirname
cp output-server.txt $dirname
cp qlen*.txt $dirname
cp /home/lca2/Desktop/delay_client_server.txt $dirname
cp /home/lca2/Desktop/delay_server_client.txt $dirname
cp /home/lca2/Desktop/Old_Messages.txt $dirname
cp network-quic.py $dirname
./num_open_streams < $dirname/output-client.txt > $dirname/num_open_streams.txt
python3 plot_num_open_streams.py $dirname/num_open_streams.txt &
./preprocess_message_receipt  $dirname/delay_client_server.txt < $dirname/output-server.txt > $dirname/ephemeral_message_receipt.txt
python3 plot_ephemeral_message_receipt.py $dirname/ephemeral_message_receipt.txt &
python3 plot_delay_cdf.py $dirname/delay_client_server.txt &
#python3 plot_queue_length_rh.py $dirname/qlen_rh.txt &
python3 plot_queue_length_ri.py $dirname/qlen_ri.txt &
cp *.png $dirname/
./delay < $dirname/delay_client_server.txt
cp /home/lca2/Desktop/rolle-network/proto-quic/src/net/tools/quic/quic_spdy_client_base.cc $dirname/



