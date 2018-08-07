# Monitor instantenous queue length at rh

from time import sleep, time
from subprocess import *
import re

default_dir = '.'

def monitor_qlen(interval_sec=0.01):
  pat_queued =re.compile(r'backlog\s[^\s]+\s([\d]+)p')
  cmd = "tc -s qdisc show dev rh-eth1"
  ret = []
  open("qlen_rh.txt","w").write('')
  t0 = "%f" % time()
  while 1:
    p = Popen(cmd, shell=True, stdout=PIPE)
    output = p.stdout.read()
    matches = pat_queued.findall(output)
    if matches and len(matches) > 1:
      ret.append(matches[1])
      t1 = "%f" % time()
      print str(float(t1)-float(t0)), matches[1]
      open("qlen_rh.txt","a").write(str(float(t1)-float(t0))+' '+matches[1]+'\n')
    sleep(interval_sec)

monitor_qlen()

