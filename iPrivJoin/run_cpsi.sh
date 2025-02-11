# nohup bash run_cpsi.sh > run_cpsi.log 2>&1 &
# ./extern/volePSI/out/build/linux/frontend/frontend  -in sFile_deleteMe -csv -r 0 -v -cpsi -nn 10 -t 100  &
# ./extern/volePSI/out/build/linux/frontend/frontend  -in rFile_deleteMe  -csv -r 1 -v -cpsi -nn 10 -t 100 


# ./extern/volePSI/out/build/linux/frontend/frontend -perf -cpsi -nn 10 -t 1 -v

./extern/volePSI/out/build/linux/paper_exp_volepsi/cpsi_test 0 &
./extern/volePSI/out/build/linux/paper_exp_volepsi/cpsi_test 1


# sudo tc qdisc del dev lo root
# sudo tc qdisc add dev lo root handle 1: htb default 1
# sudo tc class add dev lo parent 1: classid 1:1 htb rate 75Mbit
# sudo tc qdisc add dev lo parent 1:1 handle 2:0 netem delay 20ms


#1<<14 Y,  1<<18 Y,   372500 N,  745000 N, 1<<20 N
# ./extern/volePSI/out/build/linux/paper_exp_volepsi/cpsi_test 0 372500 1024 411 &
# ./extern/volePSI/out/build/linux/paper_exp_volepsi/cpsi_test 1 372500 1024 411

# ./extern/volePSI/out/build/linux/paper_exp_volepsi/cpsi_test /root/zqz/mpc_projects/data/CFIX_1row_small.json 0 ./out.csv0 50000 &
echo "---------------------------------------------------------------"
echo "----------------------------CFI-----------------------------------"
# ./extern/volePSI/out/build/linux/paper_exp_volepsi/cpsi_test /root/zqz/mpc_projects/data/CFIX_1row.json 0 ./CFI.out0 262144 &
# ./extern/volePSI/out/build/linux/paper_exp_volepsi/cpsi_test /root/zqz/mpc_projects/data/CFIY1024.json 1 ./CFI.out1 262144

# ./extern/volePSI/out/build/linux/paper_exp_volepsi/cpsi_test /root/zqz/mpc_projects/data/CFIX_1row.json  0  ./CFI_nobatch.out0  &
# ./extern/volePSI/out/build/linux/paper_exp_volepsi/cpsi_test /root/zqz/mpc_projects/data/CFIY1024.json  1  ./CFI_nobatch.out1


# sleep 3
# echo "---------------------------------------------------------------"
# echo "----------------------------SVHN-----------------------------------"
# ./extern/volePSI/out/build/linux/paper_exp_volepsi/cpsi_test /root/zqz/mpc_projects/data/SVHNX_1row.json 0 ./SVHN.out0 30000 &
# ./extern/volePSI/out/build/linux/paper_exp_volepsi/cpsi_test /root/zqz/mpc_projects/data/SVHNY1024.json 1 ./SVHN.out1 30000


# sudo tc qdisc del dev lo root

