#!/bin/bash
# nohup bash run_config.sh > run_config.log 2>&1 &

sudo tc qdisc del dev lo root
sudo tc qdisc add dev lo root handle 1: htb default 1
sudo tc class add dev lo parent 1: classid 1:1 htb rate 1Gbps

# 定义配置数组
configs=(
    # "2458 1024 1048576 1000"
    # "4916 2048 1048576 560"
    "9831 4096 1048576 300"
    # "2458 1024 4194304 3750"
    # "4916 2048 4194304 1900"
    # "9831 4096 4194304 1000"
    # "2458 1024 16777216 14500"
    # "4916 2048 16777216 7500"
    # "9831 4096 16777216 3750"
    # "2458 1024 10000 20"
    

)

# 临时文件路径
config_file="config_1048576_4096_1000_LAN.toml"

# 循环遍历每组配置进行实验
for i in "${!configs[@]}"; do
    # 提取配置参数
    IFS=' ' read -r bins pa_elems pb_elems max_in_bin<<< "${configs[$i]}"
    
    # 修改 config 文件
    {
        echo "bins = $bins"
        echo "funcs = 2"
        echo 'address = "localhost:10051"'
        echo "pa_elems = $pa_elems"
        echo "pa_features = 1"
        echo "pb_elems = $pb_elems"
        echo "pb_features = 500"
        echo "max_in_bin = $max_in_bin"
    } > "$config_file"
    
    # 运行实验命令
    echo "------------------------LAN--------------------------------------------"
    echo "Running experiment $((i + 1)) with config:"
    cat "$config_file"
    
    ./build/iPrivJoin config_1048576_4096_1000_LAN
    echo "--------------------------------------------------------------------"
    # 可选：暂停一段时间
    sleep 1m
done


sudo tc qdisc del dev lo root
sudo tc qdisc add dev lo root handle 1: htb default 1
sudo tc class add dev lo parent 1: classid 1:1 htb rate 75Mbit
sudo tc qdisc add dev lo parent 1:1 handle 2:0 netem delay 20ms


# 临时文件路径
config_file="./test/config/config_1048576_4096_1000_WAN.toml"

# 循环遍历每组配置进行实验
for i in "${!configs[@]}"; do
    # 提取配置参数
    IFS=' ' read -r bins pa_elems pb_elems max_in_bin<<< "${configs[$i]}"
    
    # 修改 config 文件
    {
        echo "bins = $bins"
        echo "funcs = 2"
        echo 'address = "localhost:10051"'
        echo "pa_elems = $pa_elems"
        echo "pa_features = 1"
        echo "pb_elems = $pb_elems"
        echo "pb_features = 500"
        echo "max_in_bin = $max_in_bin"
    } > "$config_file"
    
    # 运行实验命令
    echo "------------------------WAN--------------------------------------------"
    echo "Running experiment $((i + 1)) with config:"
    cat "$config_file"
    
    ./build/iPrivJoin config_1048576_4096_1000_WAN
    echo "--------------------------------------------------------------------"
    # 可选：暂停一段时间
    sleep 1m
done

sudo tc qdisc del dev lo root