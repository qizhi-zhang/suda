# nohup bash run.sh >run.log 2>&1 &
for num_payload in 1048576 4194304 16777216
do
    for num_query in 1024 2048 4096
    do
        for payload_size in 256 1000
        do
            echo "num_payload=$num_payload, num_query=$num_query, payload_size=$payload_size"
            ./bin/pirexamples -b 1 -l $num_query -n $num_payload -x $payload_size -c 0
        done
    done
done
