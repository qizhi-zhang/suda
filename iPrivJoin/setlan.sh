sudo tc qdisc del dev lo root
sudo tc qdisc add dev lo root handle 1: htb default 1
sudo tc class add dev lo parent 1: classid 1:1 htb rate 1Gbps