import re
import pandas as pd


def performance(log_file, performance_file):
        
    # 读取日志文件
    with open(log_file, 'r') as file:
        log_content = file.read()

    # 定义要提取的关键字
    keywords = ['num_payload=', 'num_query=', 'payload_size=', 'server_init_time=', 'server_time=', 'client_time=',
                'server_mem=',  'client_mem=', 
                'Query size: ',
                'Response size: ']

    # 初始化空字典存储提取的值
    data = {key: [] for key in keywords}

    # 使用正则表达式提取关键字对应的值
    for key in keywords:
        if key in ['num_payload=', 'num_query=', 'payload_size=']:
            matches = re.findall(f'{key}(\d+\.?\d+?)', log_content)
        else:
            matches = re.findall(f'{key}(.*\n)', log_content)
            matches = [r.replace("\n", "") for r in matches]
            matches = [r.replace("ms", "") for r in matches]
            matches = [r.replace("MB", "") for r in matches]
            matches = [r.replace("KBytes", "") for r in matches]
            matches = [r.replace(" ", "") for r in matches]
        data[key] = matches
        print(key)
        print(len(matches))

    # 将提取的数据组成表格
    print(data)
    df = pd.DataFrame(data, dtype="float")

    print(df)

    df.to_csv(performance_file)
    
if __name__=='__main__':
    performance('run.log', "./performance.csv")
