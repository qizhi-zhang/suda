## python3.9
import pandas as pd
import numpy as np

exp="CPSI"

data0 = None
data1 = None

def collect(dataset):
    if dataset=="CFI":
        data0 = pd.read_csv("./CFIX.csv0", dtype="int64", header=None)
        data1 = pd.read_csv("./CFIX.csv1", dtype="int64", header=None)
    if dataset=="SVHN":
        data0 = pd.read_csv("./SVHN.out0", dtype="int64", header=None)
        data1 = pd.read_csv("./SVHN.out1", dtype="int64", header=None)



    print(data0)
    print(data1)
    data = data0 ^ data1


    # data = data[data.iloc[:,-1]==1]
    data = np.reshape(data.values, [-1, 1305, data.shape[1]])
    print(data.shape)
    data = data * data[:,:,-1:]
    data = np.sum(data, axis=0, keepdims=False)
    print(data)
    print(data[:,-1].tolist())
    print(sum(data[:,-1].tolist()))

collect("CFI")
# collect("SVHN")

