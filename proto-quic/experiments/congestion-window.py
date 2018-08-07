
import re
import sys

filename = sys.argv[1]
with open(filename) as f:
    lines = f.readlines()

for line in lines:
    pattern = re.compile(r'congestion_window')
    matches = pattern.findall(line)
    if len(matches) > 0:
        print(matches)

