import socket
import time

HOST = ''
PORT = 5001

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
s.bind((HOST, PORT))

count = 0
count_valid = 0

while True:
    data, addr = s.recvfrom(1024)
    nums = data.decode().split(' ')
    timestamp = float(nums[0])
    id = int(nums[1])

    current_time = time.time()
    delay = (current_time - timestamp) * 1000
    count += 1
    if (delay < 100):
        count_valid += 1
    print(current_time, delay, count, count_valid, id, flush=True)

