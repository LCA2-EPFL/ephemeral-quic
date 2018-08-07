#!/usr/bin/env python3
from mininet.net import Mininet
from mininet.node import Controller
from mininet.link import Intf, TCIntf, TCLink
from mininet.cli import CLI
from mininet.log import setLogLevel, info

from mininet.term import makeTerm, makeTerms
import time
import os.path

def createNetwork():
	#send rate at each link in Mbps
	bwg = 1#000 #1000 #in Mbps
	bwbn = 1#000 #1000 #25 #in Mbps
	loss = 80 #1 #2.5 #10 #1 #in %
	mqs = 100 #0 #1000 #max queue size of interfaces
	dly = '2.5ms'#'2.5ms 0.5ms'#'1ms 0.5ms' #can take all tc qdisc delay distribution formulations

	#create empty network
	net = Mininet(intf=TCIntf)

	info( '\n*** Adding controller\n' )
	net.addController( 'c0' ) #is it ok ?

	#add host to topology
	ht = net.addHost( 'ht', ip='10.10.0.1/24' )
	hu = net.addHost( 'hu', ip='10.10.0.2/24' )
	it = net.addHost( 'it', ip='10.20.0.1/24' )
	iu = net.addHost( 'iu', ip='10.20.0.2/24' )

	rh = net.addHost('rh', ip='10.10.0.10/24')
	ri = net.addHost('ri', ip='10.20.0.20/24')

	info('\n** Adding Switches\n')
	# Adding 2 switches to the network
	sw1 = net.addSwitch('sw1')
	sw2 = net.addSwitch('sw2')

	info('\n** Creating Links \n')
	#create link beetween the network
	link_ht_sw1 = net.addLink( ht, sw1)
	link_hu_sw1 = net.addLink( hu, sw1)
	link_rh_sw1 = net.addLink( rh, sw1, intfName1='rh-eth0')

	link_it_sw2 = net.addLink( it, sw2)
	link_iu_sw2 = net.addLink( iu, sw2)
	link_ri_sw2 = net.addLink( ri, sw2, intfName1='ri-eth0')

	link_rh_ri  = net.addLink( rh, ri, intfName1='rh-eth1', intfName2='ri-eth1')


	#set bandwith
	link_ht_sw1.intf1.config( bw = bwbn, max_queue_size = mqs)
	link_hu_sw1.intf1.config( bw = bwbn, max_queue_size = mqs)
	link_rh_sw1.intf1.config( bw = bwbn, max_queue_size = mqs) #max_queue_size is hardcoded low to prevent bufferbloat, too high queuing delays

	link_it_sw2.intf1.config( bw = bwg, max_queue_size = mqs)
	link_iu_sw2.intf1.config( bw = bwg, max_queue_size = mqs)
	link_ri_sw2.intf1.config( bw = bwg, max_queue_size = mqs, delay=dly) #delay is set at ri on both interfaces

	# link_rh_ri.intf1.config(  bw = bwg, max_queue_size = 10, loss=loss) #loss is set at rh on its interface to ri only
	link_rh_ri.intf1.config(  bw = bwg, max_queue_size = mqs, loss=loss) #loss is set at rh on its interface to ri only

	link_ht_sw1.intf2.config( bw = bwbn, max_queue_size = mqs)
	link_hu_sw1.intf2.config( bw = bwbn, max_queue_size = mqs)
	link_rh_sw1.intf2.config( bw = bwbn, max_queue_size = mqs)

	link_it_sw2.intf2.config( bw = bwg, max_queue_size = mqs)
	link_iu_sw2.intf2.config( bw = bwg, max_queue_size = mqs)
	link_ri_sw2.intf2.config( bw = bwg, max_queue_size = mqs)

	link_rh_ri.intf2.config(  bw = bwg, max_queue_size = mqs,  delay=dly) #delay is set at ri on both interfaces

	net.start()

	info( '\n*** Configuring hosts\n' )

	rh.cmd('ifconfig rh-eth1 10.12.0.10 netmask 255.255.255.0') #reconfiguring mutiples intefaces host to prevent mininet strange initialisation behaviors
	rh.cmd('ifconfig rh-eth0 10.10.0.10 netmask 255.255.255.0')
	rh.cmd('echo 1 > /proc/sys/net/ipv4/ip_forward') #enable forwarding at routers

	ri.cmd('ifconfig ri-eth1 10.12.0.20 netmask 255.255.255.0') #reconfiguring mutiples intefaces host to prvent mininet strange initialisation behaviors
	ri.cmd('ifconfig ri-eth0 10.20.0.20 netmask 255.255.255.0')
	ri.cmd('echo 1 > /proc/sys/net/ipv4/ip_forward') #enable forwarding at routers

	#configure host default gateways
	ht.cmd('ip route add default via 10.10.0.10')
	hu.cmd('ip route add default via 10.10.0.10')
	it.cmd('ip route add default via 10.20.0.20')
	iu.cmd('ip route add default via 10.20.0.20')

	#configure router routing tables
	rh.cmd('ip route add default via 10.12.0.20')
	ri.cmd('ip route add default via 10.12.0.10')

        # weiyu:
        iu.cmd('touch server.pcap')
        hu.cmd('touch client.pcap')


        rh.cmd('tc qdisc del dev rh-eth1 root')
        rh.cmd('tc qdisc add dev rh-eth1 root netem loss gemodel 0.2% 20% 90% 0.1% limit ' + str(mqs))
        #rh.cmd('tc qdisc add dev rh-eth1 root netem loss gemodel 0.2% 2% 90% 2% limit 10')
        #rh.cmd('tc qdisc add dev rh-eth1 root netem loss gemodel 0.1% 1% 90% 2% limit 1000')
        #rh.cmd('tc qdisc add dev rh-eth1 root netem loss gemodel 0.5% 2% 90% 2% limit 1000')

       # rh.cmd('python ./monitor_qlen_rh.py &')
        rh.cmd('xterm -xrm \'XTerm.vt100.allowTitleOps: false\' -T rh -e \'sudo python ./monitor_queue.py\' &')
       # ri.cmd('python ./monitor_qlen_ri.py &')
        ri.cmd('xterm -xrm \'XTerm.vt100.allowTitleOps: false\' -T ri -e \'sudo python ./monitor_qlen_ri.py\' &')
        #it.cmd('xterm -xrm \'XTerm.vt100.allowTitleOps: false\' -T it -e \'sudo ./tcpserver 6666 > tcp-output-server.txt\' &')

        #ht.cmd('xterm -xrm \'XTerm.vt100.allowTitleOps: false\' -T ht -e \'sleep 10; sudo ./tcpclient 10.20.0.1 6666 > tcp-output-client.txt\' &')

       # iu.cmd('tshark -i iu-eth0 -w server.pcap &')
        iu.cmd('xterm -xrm \'XTerm.vt100.allowTitleOps: false\' -T iu -e \'sudo tshark -i iu-eth0 -w server.pcap\' &')
       # iu.cmd('./server.sh &')
        # iu.cmd('xterm -xrm \'XTerm.vt100.allowTitleOps: false\' -T iu -e \'sudo ./baseline_server.sh > output-server.txt\' &')
        iu.cmd('xterm -xrm \'XTerm.vt100.allowTitleOps: false\' -T iu -e \'python3 tcp_server.py > tcp-output-server.txt\' &')
       # hu.cmd('tshark -i hu-eth0 -w client.pcap &')
        hu.cmd('xterm -xrm \'XTerm.vt100.allowTitleOps: false\' -T hu -e \'sudo tshark -i hu-eth0 -w client.pcap\' &')
       # hu.cmd('./client.sh &')
        # hu.cmd('xterm -xrm \'XTerm.vt100.allowTitleOps: false\' -T hu -e \'sleep 5; sudo ./baseline_client.sh > output-client.txt\' &')
        hu.cmd('xterm -xrm \'XTerm.vt100.allowTitleOps: false\' -T hu -e \'python3 tcp_client.py > tcp-output-client.txt \' &')


	it.cmd('ethtool -K it-eth0 tx off sg off tso off') #disable TSO on TCP on defaul TCP sender need to be done on other host if sending large TCP file from other nodes

	method = 'tcp' #case selector varible for the flow used by smart-grid 'udp' = FRED

	logFolder = "../Estimations/wifiTer/"+ method + "/" #folder where log files and metrics will be saved
	# timeout = 10 #durantion of test

	#if not os.path.exists(logFolder):
	try:
		os.makedirs(logFolder) #error if folder already exist in order to prevent exidental overwirie
	except:
		print("File already exists.")

        # makeTerms([iu, hu, rh, ri], "host")

	#hu.cmd("bash /home/lca2/Desktop/server.sh")
	time.sleep(1)

	#iu.cmd("bash /home/lca2/Desktop/client-network.sh")
	time.sleep(1)

	"""it.cmd("python3 tcpserver.py &> "+logFolder+"it.txt &")
	time.sleep(1)
	ht.cmd("python3 tcpclient.py --ip 10.20.0.1 --port 4242 -s "+logFolder+"ht- -t "+str(timeout)+" &> "+logFolder+"ht.txt &")

	#potential second flow in the reverse direction of the first

	#ht.cmd("python3 tcpserver.py --ip 10.10.0.1 --port 4243 &> "+logFolder+"ht2.txt &")
	#time.sleep(1)
	#it.cmd("python3 tcpclient.py --ip 10.10.0.1 --port 4243 -s "+logFolder+"it2- -t "+str(timeout)+" &> "+logFolder+"it2.txt &")

	#smart grid data will be transported by TCP, delay will be recorded
	if method == 'tcp':
		info(method)
		iu.cmd("python3 delayReceiver.py --tcp --ip 10.20.0.2 -p 4242 -s "+logFolder+"iu- -t "+str(timeout)+" &> "+logFolder+"iu.txt &")
		time.sleep(1)
		hu.cmd("python3 tcpsender.py -t "+str(timeout)+" &> "+logFolder+"hu.txt &")

	#smart grid data will be transported by FRED, delay will be recorded
	elif method == 'udp':
		info(method)
		iu.cmd("python3 delayReceiver.py --ip 10.20.0.2 -p 4242 -s "+logFolder+"iu- -t "+str(timeout)+" &> "+logFolder+"iu.txt &")
		time.sleep(1)
		hu.cmd("python3 udpsender.py -s "+logFolder+"hu- -t "+str(timeout)+" &> "+logFolder+"hu.txt &")

	else:
		info("method unknown")
		net.stop()
		return

	#wainting until test end
	info('\n*** Sleeping\n')
	for i in range(int(timeout)):
		time.sleep(60)
		info("**slept "+str(i+1))"""

	# Enable the mininet> prompt if uncommented
	info('\n*** Running CLI\n')
	CLI(net)

	#kill xterms in case some where opened
	#ht.cmd("killall xterm")
	#it.cmd("killall xterm")
	# hu.cmd("killall xterm")
	iu.cmd("killall xterm")
	# stops the simulation
	#net.stop()


if __name__ == '__main__':
	setLogLevel( 'info' )
	#setLogLevel( 'output' )
	createNetwork()
