# Suda: An Efficient and Secure Unbalanced Data Alignment Framework for Vertical Privacy-Preserving Machine Learning

This is the code for expierments in the paper "Suda: An Efficient and Secure Unbalanced Data Alignment Framework for
Vertical Privacy-Preserving Machine Learning". 

# Requerments:

Debian GNU/Linux 10 (buster) +,    gcc 11.5.0+ and clang 18.1.8+,   memory 750GB


# Run  expierments for our work:


## Install dependencies

```
cd mpc_projects
cd src/third_party
bash gmp.get
bash ntl.get
bash libsodium.get
bash seal.get
bash pailliercryptolib.get
bash volepsi.get

```

If meet the error like

```
CMake Error at .../suda/volepsi/out/coproto/thirdparty/getBoost.cmake:67 (file):
file DOWNLOAD HASH mismatch
for file: [.../suda/volepsi/out/boost_1_86_0.tar.bz2]
expected hash: [1bed88e40401b2cb7a1f76d4bab499e352fa4d0c5f31c0dbae64e24d34d7513b]
actual hash: [79e6d3f986444e5a80afbeccdaf2d1c1cf964baa8d766d20859d653a16c39848]
```

in running `bash volepsi.get` , replace the line 8 in getBoost.cmake to

```
set(URL "https://archives.boost.io/release/1.86.0/source/boost_1_86_0.tar.bz2")
```

and re-run

```
bash volepsi.get
```


## Setup:

```
cd mpc_projects
bash setup.sh
```

## Run expierments in Table 2

```
bash run_psi_to_share_test_size.sh > run_psi_to_share_test_size.log 2>&1 &
```

## Run expierments in Table 3

```
bash run_psi_to_share_test_payload.sh > run_psi_to_share_test_payload.log 2>&1 &
```

## Run expierments in Table 4

```
bash run_psi_to_share_test_interratio.sh > run_psi_to_share_test_interratio.log 2>&1 &
```


## Run expierments in Table 5

```
nohup bash run_batchpir_test.sh >batch_pir.log  2>&1 &
```

## Run expierments in Table 1 (Suda)

### Part Secure Data Alignment 

Firstly, download and preprocess the datasets to needed formula:

```
bash preprocess_dataset.sh
```

Then you can run psi to share using preprocessed datasets:

```
nohup bash run_psi_to_share_using_files_test.sh > run_psi_to_share_using_files_test.log 2>&1 &
```

### Part Secure Training

Finnaly, you can run mpc-lr on the aliged secret-share dataset:

```
bash run_mpclr.sh
```


# Run  expierments for comparing work:


# Run PIRANA

## Setup PIRANA

```
cd PIRANA
bash setup.sh
```

## Run expierments in Table 5

```
cd PIRANA
nohup bash run.sh >run.log 2>&1 &
```

### collect permormances: 
```
python3 -m venv .venv
source .venv/bin/activate
pip3 install pandas
python3 performance.py
```

# Run CPSI
## Setup:

Edit `volepsi/setup.sh`, set `CMAKE_CXX_COMPILER`  and `CMAKE_C_COMPILER`, then

```
cd volepsi
bash setup.sh
```

If meet the error

```
CMake Error at .../suda/volepsi/out/coproto/thirdparty/getBoost.cmake:67 (file):
file DOWNLOAD HASH mismatch
for file: [.../suda/volepsi/out/boost_1_86_0.tar.bz2]
expected hash: [1bed88e40401b2cb7a1f76d4bab499e352fa4d0c5f31c0dbae64e24d34d7513b]
actual hash: [79e6d3f986444e5a80afbeccdaf2d1c1cf964baa8d766d20859d653a16c39848]
```

replace the line 8 in getBoost.cmake to

```
set(URL "https://archives.boost.io/release/1.86.0/source/boost_1_86_0.tar.bz2")
```

and re-run

```
bash setup.sh
cd ..
```

## Run expierments in Table 1 (CPSI)

### Part Secure Data Alignment 
```
cd volePSI
bash run_cpsi.sh
```

### Part Secure Training

```
bash run_mpclr.sh
```




# Run iPrivateJoin
## Setup

```
cd iPrivJoin
bash setup_paper_exp.sh
cd ..
```



