#ifndef FILEIO
#define FILEIO
#include "json.hpp"
#include <iostream>
#include <fstream> 
#include <string>
#include "cryptoTools/Common/BitVector.h"
using json = nlohmann::json;
// using namespace std;
// using namespace osuCrypto;





inline int read_cols_server(std::string file_name, std::vector<std::string> &server_ids, std::vector<std::vector<int64_t>>& data) {
    std::ifstream file(file_name); // 打开 JSON 文件
    json json_obj;
    file >> json_obj; // 将 JSON 数据解析为 nlohmann::json 对象
    file.close(); // 关闭文件
    size_t col_num = json_obj.size();
    std::cout<<"json_obj.size()="<<col_num<<std::endl;
    // std::vector<std::string> server_ids;
    // std::vector<std::vector<int64_t>> data(col_num-1); 
    data.resize(col_num-1);
    // vector<string> client_ids;
    // vector<bool> client_labels;

    // 遍历 JSON 对象并打印值
    size_t j=0;
    for (auto& col : json_obj.items()) {
        // std::cout << "Key: " << element.key() << ", Value: " << element.value() << std::endl;
            std::string key = col.key();
            // std::cout << "Key: " << key << std::endl;  
             if (key=="index")
             {
                auto server_ids_ = col.value().get<std::vector<int64_t>>();
                server_ids.resize(server_ids_.size());
                for (size_t i=0; i<server_ids_.size(); i++)
                {
                    server_ids[i] = std::to_string(server_ids_[i]);
                }
                // std::cout<<"server_ids.size()="<<server_ids.size()<<std::endl;
             }
             else
             {
                // std::cout<<"j="<<j<<std::endl;
                data[j] = col.value().get<std::vector<int64_t>>();
                // std::cout<<"data[j].size()="<<data[j].size()<<std::endl;
                j++;
            }
    }

    return 0;
}


inline int read_label_client(std::string file_name, std::vector<int64_t>& client_ids, std::vector<bool>& labels)
{
    std::ifstream file(file_name); // 打开 JSON 文件
    json json_obj;
    file >> json_obj; // 将 JSON 数据解析为 nlohmann::json 对象
    file.close(); // 关闭文件
    // std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
    client_ids=json_obj["index"].get<std::vector<int64_t>>();
    // std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
    // labels = json_obj["label"].get<std::vector<bool>>();
    labels = json_obj["label"].get<std::vector<bool>>();
    // std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
    return 0;
}



inline int read_label_client(std::string file_name, std::vector<std::string>& client_ids, std::vector<bool>& labels)
{
    std::ifstream file(file_name); // 打开 JSON 文件
    json json_obj;
    file >> json_obj; // 将 JSON 数据解析为 nlohmann::json 对象
    file.close(); // 关闭文件

    std::vector<int64_t> client_ids_=json_obj["index"].get<std::vector<int64_t>>();
    client_ids.resize(client_ids_.size());
    for (size_t i=0; i<client_ids_.size(); i++)
    {
        client_ids[i] = std::to_string(client_ids_[i]);
    }
    // labels = json_obj["label"].get<std::vector<bool>>();
    labels = json_obj["label"].get<std::vector<bool>>();
    return 0;
}





inline int read_rows(std::string file_name, std::vector<int64_t> & server_ids, std::vector<std::vector<int64_t>> & data) {
    // std::ifstream file("/root/zqz/mpc_projects/data/CFIX_rows.json"); // 打开 JSON 文件
    std::ifstream file(file_name);
    json json_obj;

    file >> json_obj; // 将 JSON 数据解析为 nlohmann::json 对象
    file.close(); // 关闭文件

    server_ids=json_obj["index"].get<std::vector<int64_t>>();
    data=json_obj["data"].get<std::vector<std::vector<int64_t>>>();

        std::cout<<"server_ids.size()="<<server_ids.size()<<std::endl;
        std::cout<<"data.size()="<<data.size()<<std::endl;
        std::cout<<"data[0].size()="<<data[0].size()<<std::endl;

    return 0;
}



template <typename T>
inline int read_1row(std::string file_name, std::vector<T> & server_ids, std::vector<int64_t> & data) {
    // std::ifstream file("/root/zqz/mpc_projects/data/CFIX_1row.json"); // 打开 JSON 文件
    std::ifstream file(file_name);
    json json_obj;
    // std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
    file >> json_obj; // 将 JSON 数据解析为 nlohmann::json 对象
    file.close(); // 关闭文件
// std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
    server_ids=json_obj["index"].get<std::vector<T>>();
    data=json_obj["data"].get<std::vector<int64_t>>();

        std::cout<<"server_ids.size()="<<server_ids.size()<<std::endl;
        std::cout<<"data.size()="<<data.size()<<std::endl;
        // std::cout<<"data[0].size()="<<data[0].size()<<std::endl;
        std::cout<<"1.0*data.size()/server_ids.size()="<<1.0*data.size()/server_ids.size()<<std::endl;
    return 0;
}


inline int save_csv(std::vector<std::vector<int>> data, std::string file_name) {
    std::ofstream outputFile(file_name);

    if (outputFile.is_open()) {
        // std::vector<std::vector<int>> data = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};

        for (const auto& row : data) {
            for (size_t i = 0; i < row.size(); ++i) {
                outputFile << row[i];
                if (i != row.size() - 1) {
                    outputFile << ",";
                }
            }
            outputFile << "\n";
        }

        outputFile.close();
        std::cout << "Data has been written to data.csv" << std::endl;
    } else {
        std::cerr << "Error opening file for writing" << std::endl;
    }

    return 0;
}



// int save_csv(oc::Matrix<oc::u8> feature, std::vector<bool> label, osuCrypto::BitVector mFlagBit, string file_name) {
//     std::ofstream outputFile(file_name);
    
//     std::vector<int64_t> data(feature.size()/8);
//     std::memcpy(data.data(), feature.data(), feature.size());
//     size_t rows = feature.rows();
//     size_t cols = feature.cols()/8;

//     if (outputFile.is_open()) {
//         // std::vector<std::vector<int>> data = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};

//         for (size_t i=0; i<rows; i++) {
//             for (size_t j = 0; j < cols; j++) {
//                 outputFile << data[i*cols+j] << ",";
//             }
//             outputFile << label[i]<<",";
//             outputFile << mFlagBit[i]<<"\n";
//         }

//         outputFile.close();
//         std::cout << "Data has been written to "<< file_name << std::endl;
//     } else {
//         std::cerr << "Error opening file for writing" << std::endl;
//     }

//     return 0;
// }




inline int save_to_csv(oc::Matrix<oc::block> feature, std::string file_name) {
    std::ofstream outputFile(file_name);
    
    std::vector<int64_t> data(feature.size()*2);
    std::memcpy(data.data(), feature.data(), feature.size()*16);
    size_t rows = feature.rows();
    size_t cols = feature.cols()*2;

    if (outputFile.is_open()) {
        // std::vector<std::vector<int>> data = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};

        for (size_t i=0; i<rows; i++) {
            for (size_t j = 0; j < cols; j++) {
                outputFile << data[i*cols+j];
                if (j<cols-1)
                {
                    outputFile << ",";
                }
                else
                {
                    outputFile << std::endl;
                }
            }
        }

        outputFile.close();
        std::cout << "Data has been written to "<< file_name << std::endl;
    } else {
        std::cerr << "Error opening file for writing" << std::endl;
    }

    return 0;
}




// int save_csv(const oc::Matrix<oc::u8> & feature, const std::vector<bool> &label, const osuCrypto::BitVector & mFlagBit, std::ofstream& outputFile) {
//     // std::ofstream outputFile(file_name);
    
//     std::vector<int64_t> data(feature.size()/8);
//     std::memcpy(data.data(), feature.data(), feature.size());
//     size_t rows = feature.rows();
//     size_t cols = feature.cols()/8;

//     if (outputFile.is_open()) {
//         // std::vector<std::vector<int>> data = {{1, 2, 3}, {4, 5, 6}, {7, 8, 9}};

//         for (size_t i=0; i<rows; i++) {
//             for (size_t j = 0; j < cols; j++) {
//                 outputFile << data[i*cols+j] << ",";
//             }
//             outputFile << label[i]<<",";
//             outputFile << mFlagBit[i]<<"\n";
//         }

//         // outputFile.close();
//         // std::cout << "Data has been written to "<< file_name << std::endl;
//     } else {
//         std::cerr << "Error opening file for writing" << std::endl;
//     }

//     return 0;
// }
#endif