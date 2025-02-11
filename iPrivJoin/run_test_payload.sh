#!/bin/bash
# nohup bash run_test_payload.sh > run_test_payload.log 2>&1 &

sudo tc qdisc del dev lo root
sudo tc qdisc add dev lo root handle 1: htb default 1
sudo tc class add dev lo parent 1: classid 1:1 htb rate 1Gbps

# 临时文件路径
config_file="./test/config/config_payload.toml"

# 循环遍历每组配置进行实验
for feature_num in 50 100 250 500 1000
do
    # 修改 config 文件
    {
        echo "bins = 2458"
        echo "funcs = 2"
        echo 'address = "localhost:10051"'
        echo "pa_elems = 1024"
        echo "pa_features = 1"
        echo "pb_elems = 1048576"
        echo "pb_features = $feature_num"
        echo "max_in_bin = 1000"
    } > "$config_file"
    
    # 运行实验命令
    echo "------------------------LAN--------------------------------------------"
    cat "$config_file"
    
    ./build/iPrivJoin config_payload
    echo "--------------------------------------------------------------------"
    # 可选：暂停一段时间
    sleep 1m
done


sudo tc qdisc del dev lo root
sudo tc qdisc add dev lo root handle 1: htb default 1
sudo tc class add dev lo parent 1: classid 1:1 htb rate 75Mbit
sudo tc qdisc add dev lo parent 1:1 handle 2:0 netem delay 20ms

for feature_num in 50 100 250 500 1000
do
    # 修改 config 文件
    {
        echo "bins = 2458"
        echo "funcs = 2"
        echo 'address = "localhost:10051"'
        echo "pa_elems = 1024"
        echo "pa_features = 1"
        echo "pb_elems = 1048576"
        echo "pb_features = $feature_num"
        echo "max_in_bin = 1000"
    } > "$config_file"
    
    # 运行实验命令
    echo "------------------------WAN--------------------------------------------"
    cat "$config_file"
    
    ./build/iPrivJoin config_payload
    echo "--------------------------------------------------------------------"
    # 可选：暂停一段时间
    sleep 1m
done

sudo tc qdisc del dev lo root