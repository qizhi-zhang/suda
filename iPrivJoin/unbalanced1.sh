#!/bin/bash

# 定义配置数组
configs=(
    "2516583 1048576 1024"
    "2516583 1048576 2048"
    "2516583 1048576 4096"
    "10066330 4194304 1024"
    "10066330 4194304 2048"
    "10066330 4194304 4096"
    "40265319 16777216 1024"
    "40265319 16777216 2048"
    "40265319 16777216 4096"
)

# 临时文件路径
config_file="./test/config/config.toml"

# 循环遍历每组配置进行实验
for i in "${!configs[@]}"; do
    # 提取配置参数
    IFS=' ' read -r bins pa_elems pb_elems <<< "${configs[$i]}"
    
    # 修改 config 文件
    {
        echo "bins = $bins"
        echo "funcs = 2"
        echo 'address = "localhost:10051"'
        echo "pa_elems = $pa_elems"
        echo "pa_features = 50"
        echo "pb_elems = $pb_elems"
        echo "pb_features = 1"
        echo "max_in_bin = 3"
    } > "$config_file"
    
    # 运行实验命令
    echo "Running experiment $((i + 1)) with config:"
    cat "$config_file"
    
    ./build/iPrivJoin config
    
    # 可选：暂停一段时间
    # sleep 1
done
