#include "joinData.h"
#include <csignal>
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <volePSI/Defines.h>
#include "csv.hpp"


void joinData::joinData1(const PsiAnalyticsContext &context)
{
    auto &configs = context.role == PA ? context.pa_config : context.pb_config;
    volePSI::PRNG prng({ 5260, 1080 });
    switch (context.role) {
    case PA:
        features = Matrix(context.pa_elems, context.pa_features);
        prng.get(features.data(), context.pa_elems * context.pa_features);
        ids = std::vector<uint64_t>(context.pa_elems);
        prng.get(ids.data(), context.pa_elems);
        break;
    case PB:
        features = Matrix(context.pb_elems, context.pb_features);
        prng.get(features.data(), context.pb_elems * context.pb_features);
        ids = std::vector<uint64_t>(context.pb_elems);
        prng.get(ids.data(), context.pb_elems);
        break;
    }
    if (!context.is_test) {
        csv::CSVReader reader(context.data_file);
        for (auto &row : reader) {
            size_t i = 0;
            uint64_t id = std::atoi(row[0].get().c_str());
            ids.push_back(id);
            for (size_t j = 1; j < row.size(); j++) {
                block b;
                switch (configs[j - 1].type) {
                case NUM: {
                    to_block(row[j].get<long double>(), b);
                } break;
                case TEXT: {
                    to_block(row[j].get<std::string>(), b);
                    break;
                }
                case BOOL:
                    to_block(row[j].get<bool>(), b);
                    break;
                }
                features(i, j) = b;
            }
            i++;
        }
    }
}

// joinData::joinData(std:: config_file, std::string data_file)
// {

//     csv::CSVReader reader(data_file);
//     for (auto &row : reader) {
//         uint64_t id = std::atoi(row[0].get().c_str());
//         ids.push_back(id);
//         std::vector<feature> feature_row;
//         for (size_t index = 1; index < row.size(); index++) {
//             feature f(configs[index - 1].name, configs[index - 1].size);
//             switch (configs[index - 1].type) {
//             case INT:
//                 serialize(row[index].get<block>(), f.data, configs[index - 1].size);
//                 f.type = INT;
//                 break;
//             case FlOAT:
//                 serialize(row[index].get<float>(), f.data, configs[index - 1].size);
//                 break;
//             case TEXT:
//                 serialize(row[index].get<std::string>(), f.data, configs[index - 1].size);
//                 f.type = TEXT;
//                 break;
//             case BOOL:
//                 serialize(row[index].get<bool>(), f.data, configs[index - 1].size);
//                 f.type = BOOL;
//                 break;
//             }
//             feature_row.push_back(f);
//         }
//         features.push_back(feature_row);
//     }
// }




void joinData::joinData2(const PsiAnalyticsContext &context)
{
    auto &configs = context.role == PA ? context.pa_config : context.pb_config;
    volePSI::PRNG prng({ 5260, 1080 });
    if (context.role==PA) {
    // case PA: // client
        std::vector<int64_t> client_ids;
        std::vector<bool> labels;
        std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
        std::cout<<"context.data_file="<<context.data_file<<std::endl;
        read_label_client(context.data_file, client_ids, labels);
        size_t client_size = client_ids.size();
        std::cout<<"context.pa_elems"<<context.pa_elems<<std::endl;
        std::cout<<"client_size="<<client_size<<std::endl;
        std::cout<<"context.pa_features="<<context.pa_features<<std::endl;
        if (context.pa_elems!=client_size)
        {
            std::cerr<<"context.pa_elems!=client_size"<<std::endl;
        }
        if (context.pa_features!=1)
        {
            std::cerr<<"must have context.pa_features==1"<<std::endl;
        }
        ids = std::vector<uint64_t>(context.pa_elems);
        std::memcpy(ids.data(), client_ids.data(), client_size*8);
        std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
        features = Matrix(context.pa_elems, context.pa_features);
        for (size_t i=0; i<client_size; i++)
        {
            features(i, 0) = (block) labels[i];
        }
        std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
        // break;
    }
    else{
    // case PB:  //server
        std::vector<int64_t> payload;
        std::vector<int64_t> server_ids;
        std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
        std::cout<<"context.data_file="<<context.data_file<<std::endl;
        read_1row(context.data_file, server_ids, payload);
        size_t server_size = server_ids.size();
        size_t feature_num = payload.size()/server_size;
        std::cout<<"server_size="<<server_size<<std::endl;
        std::cout<<"feature_num="<<feature_num<<std::endl;
        if (context.pb_elems!=server_size)
        {
            std::cerr<<"context.pb_elems!=server_size"<<std::endl;
        }
        if (context.pb_features!=feature_num/2)
        {
            std::cerr<<"must have context.pb_features==feature_num/2"<<std::endl;
        }
        std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
        ids = std::vector<uint64_t>(context.pb_elems);
        std::memcpy(ids.data(), server_ids.data(), server_size*8);
        std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
        features = Matrix(context.pb_elems, context.pb_features);
        std::memcpy(features.data(), payload.data(), server_size*feature_num*8);
        std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
        // break;
    }
}


joinData::joinData(const PsiAnalyticsContext &context)
{
    if (context.is_test)
    {
        joinData1(context);
    }
    else
    {
        joinData2(context);
    }
}