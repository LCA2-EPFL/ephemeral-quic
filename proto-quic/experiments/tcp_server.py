import socket
import time

HOST = ''
PORT = 5001

tcp_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
tcp_socket.bind((HOST, PORT))
tcp_socket.listen(8)
client_socket, ip_port = tcp_socket.accept()

count = 0
count_valid = 0

buffer = ''

while True:
    data, addr = client_socket.recvfrom(1024)
    str = data.decode()
    buffer += str
    current_time = time.time()
    while (len(buffer) >= 100):
        msg = buffer[0:100]
        buffer = buffer[100:]
        nums = msg.split(' ')
        timestamp = float(nums[0])
        id = int(nums[1])
        delay = (current_time - timestamp) * 1000
        count += 1
        if (delay < 100):
            count_valid += 1
        print(current_time, delay, count, count_valid, id, flush=True)
    if (count == 36000):
        print("Finished")
        client_socket.send("response".encode())
        client_socket.close()
        tcp_socket.close()
        break

