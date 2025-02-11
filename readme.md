# Suda: An Efficient and Secure Unbalanced Data Alignment Framework for Vertical Privacy-Preserving Machine Learning

This is the code for expierments in the paper "Suda: An Efficient and Secure Unbalanced Data Alignment Framework for
Vertical Privacy-Preserving Machine Learning". 

# Requerments:

memory 750GB
gcc 11.5.0+ and clang 18.1.8+



# Setup mpc_projects (Our code):

```
cd mpc_projects
bash setup.sh

```

# Run mpc_projects (Our code):

## Table 1

Firstly, download and preprocess the datasets to needed formula:

```
cd mpc_projects/python
bash preprocess_dataset.sh
```

Then you can run psi to share using preprocessed datasets:

```
cd mpc_projects
bash run_psi_to_share_using_files_test.sh
```





# Setup PIRANA

```
cd PIRANA
bash setup.sh
```

# run PIRANA (Table 5 in paper)

cd PIRANA
nohup bash run.sh >run.log 2>&1 &

## PIRANA performance

pip install pandas
python3 performance.py

# Setup volepsi:

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

# run volepsi (Part CPSI in Table 1)

```
cd volePSI
bash run_cpsi.sh
```

Then, run mpc-lr on the aliged secret-share dataset:

```
bash run_mpclr.sh
```




# Setup iPrivateJoin

```
cd iPrivJoin
bash setup_paper_exp.sh
cd ..
```

# run mpc_projects:

