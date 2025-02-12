# nohup bash run_test_fromfiles.sh > run_test_fromfiles.log 2>&1 &



sudo tc qdisc del dev lo root
sudo tc qdisc add dev lo root handle 1: htb default 1
sudo tc class add dev lo parent 1: classid 1:1 htb rate 75Mbit
sudo tc qdisc add dev lo parent 1:1 handle 2:0 netem delay 20ms

    
# ./build/iPrivJoin  configIO_CFI_test 0 /root/zqz/mpc_projects/data/CFIY1024.json  &
# ./build/iPrivJoin  configIO_CFI_test 1 /root/zqz/mpc_projects/data/CFIX_1row_small.json     
./build/iPrivJoin  configIO_CFI 0 ../mpc_projects/data/CFIY1024.json  &
./build/iPrivJoin  configIO_CFI 1 ../mpc_projects/data/CFIX_1row.json 

# ./build/iPrivJoin  configIO_SVHN_test 0 /root/zqz/mpc_projects/data/SVHNY1024.json  &
# ./build/iPrivJoin  configIO_SVHN_test 1 /root/zqz/mpc_projects/data/SVHNX_1row_small.json 
sleep 1m

./build/iPrivJoin  configIO_SVHN 0 ../mpc_projects/data/SVHNY1024.json  &
./build/iPrivJoin  configIO_SVHN 1 ../mpc_projects/data/SVHNX_1row.json 





sudo tc qdisc del dev lo root

