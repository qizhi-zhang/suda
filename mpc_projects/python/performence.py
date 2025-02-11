import re
import pandas as pd
import numpy as np

# 读取日志文件
with open('../batch_pir.log', 'r') as file:
    log_content = file.read()

# 定义要提取的关键字
# keywords = ['num_payload=', 'num_query=', 'payload_size=', 'server_encode_time:', 
#             'Mem Usage of Server initialize=', 'client_init_time:', 'Mem Usage of Client initialize=',
#             'Gen query time: ', 'Mem Usage of Client generate query=', 'Gen response time: ',
#             'Mem Usage of Gen response=', 'Extract answer time: ', 'Mem Usage of Extract answer=',
#             'Query size: ', 'Response size: ']
keywords = ['host_log_n_data=', 'batch_size=', 'payload_size=', 'server_init_time:', 'Gen response time: ', 'client_init_time:', 'Gen query time: ', 
            'Extract answer time: ',
            'Mem Usage of Server initialize=',  'Mem Usage of Gen response=', 
            'Mem Usage of Client initialize=',
            'Mem Usage of Client generate query=', 
             'Mem Usage of Extract answer=']

# 初始化空字典存储提取的值
data = {key: [] for key in keywords}

# 使用正则表达式提取关键字对应的值
for key in keywords:
    if key in ['host_log_n_data=', 'batch_size=']:
        # matches = re.findall(f'{key}(\d+\.?\d+?)', log_content)
        matches = re.findall(f'{key}(\d+\,)', log_content)
        matches = [r.replace(",", "") for r in matches]
    else:
        matches = re.findall(f'{key}(.*\n)', log_content)
        matches = [r.replace("\n", "") for r in matches]
        matches = [r.replace("ms", "") for r in matches]
        matches = [r.replace("MB", "") for r in matches]
        matches = [r.replace(" ", "") for r in matches]
    data[key] = matches
    print(key)
    print(len(matches))

# 将提取的数据组成表格
print(data)
df = pd.DataFrame(data)

df["Mem usage of Server"]=df["Mem Usage of Server initialize="].apply(pd.to_numeric)+df["Mem Usage of Gen response="].apply(pd.to_numeric)
df["Mem usage of Client"]=(df["Mem Usage of Client initialize="].apply(pd.to_numeric)+df["Mem Usage of Client generate query="].apply(pd.to_numeric)).combine(df["Mem Usage of Extract answer="].apply(pd.to_numeric), func=max)
df.drop(["Mem Usage of Server initialize=", "Mem Usage of Gen response=", "Mem Usage of Client initialize=", "Mem Usage of Client generate query=", "Mem Usage of Extract answer="], axis=1, inplace=True)

df["server time"]=df["server_init_time:"].apply(pd.to_numeric)+df["Gen response time: "].apply(pd.to_numeric)
df["client time"]=df["client_init_time:"].apply(pd.to_numeric)+df["Gen query time: "].apply(pd.to_numeric)+df["Extract answer time: "].apply(pd.to_numeric)
df.drop(["Gen response time: ", "client_init_time:", "Gen query time: ", "Extract answer time: "], axis=1, inplace=True)


print(df)

df.to_csv("./performance.csv")
