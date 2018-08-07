import socket
import time

HOST = '10.20.0.2'
PORT = 5001

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST, PORT))

PACKET_SIZE = 100
NUM_PACKETS = 36000
for i in range(0, NUM_PACKETS):
    generation_time = time.time()
    s.send('{gtime:f} {num:0{width}d}'.format(num=i,width = PACKET_SIZE-18,time = time.time(),gtime =     generation_time).encode('utf8'))
    print(time.time(), i, flush=True)
    time.sleep(0.1)

recv_data = s.recv(1024)
print(recv_data.decode())

s.close()
print('all messages sent')
