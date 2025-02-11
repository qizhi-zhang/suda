#include <assert.h>
#include <getopt.h>

#include <iostream>
#include <map>
#include <unordered_set>

#include "client.h"
#include "pir_parms.h"
#include "server.h"
#include "test.h"
#include "mem_usage.h"
// #define DEBUG
using namespace std;

void show_usage() { std::cout << "**********Usage**************" << std::endl; }

void single_pir_main(const uint64_t num_payloads, const uint64_t payload_size) {
  PirParms pir_parms(num_payloads, payload_size);
  Server server(pir_parms, true);
  Client client(pir_parms);
  std::stringstream keys = client.save_keys();
  server.set_keys(keys);
  // only for debug

  auto sk = client.send_secret_keys();
  server.set_decryptor(sk);

  Timer timer;
  timer.reset();

  uint32_t index = rand() % num_payloads;
  std::stringstream query = client.gen_query(index);
  auto query_time = timer.elapsed();

  timer.reset();
  std::stringstream response = server.gen_response(query);
  auto response_time = timer.elapsed();

  timer.reset();
  std::vector<std::vector<uint64_t>> answer = client.extract_answer(response);
  auto extract_time = timer.elapsed();

  std::cout << "------------------------------------" << std::endl;
  std::cout << "Performance: " << std::endl;
  std::cout << "Gen query time: " << query_time << " ms " << std::endl;
  std::cout << "Gen response time: " << response_time << " ms " << std::endl;
  std::cout << "Extract answer time: " << extract_time << " ms " << std::endl;

  std::cout << "Query size: " << query.str().size() / 1024.0 << " KBytes"
            << std::endl;
  std::cout << "Response size: " << response.str().size() / 1024.0 << " KBytes"
            << std::endl;
  test_pir_correctness(server, answer, index, pir_parms);
}

void batch_pir_main(const uint64_t num_payloads, const uint64_t payload_size,
                    const uint64_t num_query, const bool is_batch,
                    const bool is_compress) {
  std::cout << "Start batch PIR! " << std::endl;

  Timer timer;
  float mem_usg_end, mem_usg_start;


  timer.reset();
  mem_usg_start=GetMemoryUsage();
  PirParms pir_parms(num_payloads, payload_size, num_query, is_batch,
                     is_compress);
  mem_usg_end=GetMemoryUsage();
  float parms_init_mem = mem_usg_end-mem_usg_start;
  auto parms_init_time = timer.elapsed();


  timer.reset();
  mem_usg_start=GetMemoryUsage();
  Client batch_client(pir_parms);
  auto client_init_time = timer.elapsed();
  mem_usg_end=GetMemoryUsage();
  float client_init_mem = mem_usg_end-mem_usg_start;

  std::stringstream keys = batch_client.save_keys();


  timer.reset();
  mem_usg_start = mem_usg_end;
  Server batch_server(pir_parms, is_batch, true);
  auto server_init_time = timer.elapsed();
   mem_usg_end=GetMemoryUsage();
     float server_init_mem = mem_usg_end-mem_usg_start;
  batch_server.set_keys(keys);


// std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
  // random generate query
  std::vector<uint32_t> batch_query_index(num_query);
  for (auto &q : batch_query_index) {
    q = rand() % num_payloads;
  }
  // std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
  timer.reset();
  mem_usg_start = mem_usg_end;
  std::stringstream query = batch_client.gen_batch_query(batch_query_index);
  auto query_time = timer.elapsed();
     mem_usg_end=GetMemoryUsage();
     float query_mem = mem_usg_end-mem_usg_start;
// std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
  timer.reset();
  mem_usg_start = mem_usg_end;
  std::stringstream response = batch_server.gen_batch_response(query);
  auto response_time = timer.elapsed();
       mem_usg_end=GetMemoryUsage();
     float response_mem = mem_usg_end-mem_usg_start;
// std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
  timer.reset();
  mem_usg_start = mem_usg_end;
  std::vector<std::vector<uint64_t>> answer =
      batch_client.extract_batch_answer(response);
  auto extract_time = timer.elapsed();
         mem_usg_end=GetMemoryUsage();
     float extract_mem = mem_usg_end-mem_usg_start;



  std::cout << "------------------------------------" << std::endl;
  std::cout << "Performance: " << std::endl;
#ifdef DEBUG
  std::cout<< "parms init time:" << parms_init_time << "ms" << std::endl;
  std::cout <<" server_init_time:" << server_init_time << " ms " << std::endl;
  std::cout<<"Mem Usage of Server initialize="<<server_init_mem<<"MB"<<std::endl;

  std::cout <<" client_init_time:" << client_init_time << " ms " << std::endl;

std::cout<< "Mem Usage of parms initialize=" << parms_init_mem << "MB" << std::endl;
std::cout<<"Mem Usage of Client initialize="<<client_init_mem<<"MB"<<std::endl;


  std::cout << "Gen query time: " << query_time << " ms " << std::endl;
std::cout<<"Mem Usage of Client generate query="<<query_mem<<"MB"<<endl;


  std::cout << "Gen response time: " << response_time << " ms " << std::endl;
  std::cout<<"Mem Usage of Gen response="<<response_mem<<"MB"<<endl;

  std::cout << "Extract answer time: " << extract_time << " ms "
            << std::endl;
std::cout<<"Mem Usage of Extract answer="<<extract_mem<<"MB"<<endl;

#else
  std::cout <<"server_init_time=" << parms_init_time+server_init_time << " ms " << std::endl;
  std::cout <<"server_time="<< parms_init_time+server_init_time+response_time<< "ms"<<std::endl;
  std::cout<<"client_time="<<  parms_init_time+client_init_mem+query_time+extract_time<<"ms"<<std::endl;
  std::cout<<"server_mem="<< parms_init_mem+server_init_mem+response_mem<<"MB"<<std::endl;
  std::cout<<"client_mem="<<max(parms_init_mem+client_init_mem+query_mem, extract_mem)<<"MB"<<std::endl;
#endif
  std::cout << "Query size: " << query.str().size() / 1024.0 << " KBytes"
            << std::endl;

  std::cout << "Response size: " << response.str().size() / 1024.0 << " KBytes"
            << std::endl;

  test_batch_pir_correctness(batch_server, answer, batch_query_index,
                             pir_parms);
}

int main(int argc, char *argv[]) {
  if (argc < 2) {
    show_usage();
    return 0;
  }
  // using batch pirana or single pirana
  bool is_batch = false;

  uint64_t payload_size = 256;
  uint64_t num_payloads = 16384;

  // only when is_batch == true
  uint64_t num_query = 1;
  bool is_compress = true;

  int opt;
  while (1) {
    int option_index = 0;
    static struct option long_options[] = {
        {"num_payload", optional_argument, NULL, 'n'},
        {"payload_size", optional_argument, NULL, 'x'},
        {"num_query", optional_argument, NULL, 'l'},
        {"is_batch", optional_argument, NULL, 'b'},
        {"is_compress", optional_argument, NULL, 'c'},
        {NULL, 0, NULL, 0}};

    opt = getopt_long(argc, argv, "n:x:l:b:c:vc", long_options, &option_index);
    if (opt == -1) break;

    switch (opt) {
      case 'b':
        is_batch = std::stoi(optarg);
        break;
      case 'l':
        num_query = std::stoi(optarg);
        break;
      case 'n':
        num_payloads = std::stol(optarg);
        break;
      case 'x':
        payload_size = std::stol(optarg);
        break;
      case 'c':
        is_compress = std::stol(optarg);
        break;
      case ':':
        printf("option needs a value\n");
        break;
      case '?':
        printf("unknown option: %c\n", optopt);
        return 0;
    }
  }
  if (is_batch == true) {
    batch_pir_main(num_payloads, payload_size, num_query, is_batch,
                   is_compress);
  } else {
    single_pir_main(num_payloads, payload_size);
  }
  return 0;
}