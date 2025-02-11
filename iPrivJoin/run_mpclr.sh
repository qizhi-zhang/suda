## bash run_mpclr.sh >> run_mpclr.log 2>&1 &

sudo tc qdisc del dev lo root
sudo tc qdisc add dev lo root handle 1: htb default 1
sudo tc class add dev lo parent 1: classid 1:1 htb rate 75Mbit
sudo tc qdisc add dev lo parent 1:1 handle 2:0 netem delay 20ms

echo "--------------------------------------------------------"
echo "------------------------CFI-------------------------------"

python3.9 ./mpc_lr/launcher.py -f0 /root/zqz/iPrivJoin/CFI.out0 -f1 /root/zqz/iPrivJoin/CFI.out1 --epochs 100

sleep 3

echo "--------------------------------------------------------"
echo "------------------------SVHN-------------------------------"

python3.9 ./mpc_lr/launcher.py -f0 /root/zqz/iPrivJoin/SVHN.out0 -f1 /root/zqz/iPrivJoin/SVHN.out1 --epochs 100

sudo tc qdisc del dev lo root