// #include "cryptoTools/Common/Timer.h"
// #include "cryptoTools/Common/CLP.h"
// #include "perf.h"
// #include "cryptoTools/Network/IOService.h"
#include "coproto/Socket/AsioSocket.h"
// #include "volePSI/RsPsi.h"
#include "volePSI/RsCpsi.h"
// #include "volePSI/SimpleIndex.h"
#include "tests/Common.h"
#include "file_io.h"
#include "mem_usage.h"
#include "netio.h"
// #include <coproto/Socket/AsioSocket.h>
// #include "libdivide.h"
using namespace oc;
using namespace volePSI;;
// using namespace oc;


// void CPSI_test(oc::CLP& cmd)
void CPSI_test()
{
	// auto n = 1ull << cmd.getOr("nn", 10);
	// auto t = cmd.getOr("t", 1ull);
	// //auto mal = cmd.isSet("mal");
	// auto v = cmd.isSet("v");
	// auto nt = cmd.getOr("nt", 0);
	int n=1<<10;
	int t=1;
	bool v=true;
	int nt=0;

	auto socket = cp::LocalAsyncSocket::makePair();


	RsCpsiReceiver recv;
	RsCpsiSender send;
	RsCpsiReceiver::Sharing rs;
	RsCpsiSender::Sharing ss;

	recv.init(n, n, 0, 40, ZeroBlock, nt);
	send.init(n, n, 0, 40, ZeroBlock, nt);


	std::vector<block> recvSet(n), sendSet(n);
	PRNG prng(ZeroBlock);
	prng.get<block>(recvSet);
	prng.get<block>(sendSet);
	Timer timer, s, r;

	recv.setTimer(r);
	send.setTimer(s);

	timer.setTimePoint("");
	for (u64 i = 0; i < t; ++i)
	{
		auto p0 = send.send(sendSet, {}, ss, socket[0]);
		auto p1 = recv.receive(recvSet, rs, socket[1]);

		auto r = macoro::sync_wait(macoro::when_all_ready(std::move(p0), std::move(p1)));
		std::get<0>(r).result();
		std::get<1>(r).result();

		//ev.eval(p0, p1);
	}
	timer.setTimePoint("end");


	if (v)
	{

		std::cout << timer << "\ns\n" << s << "\nr" << r << std::endl;
		std::cout << "comm " << socket[0].bytesSent() << " + " << socket[1].bytesSent()
			<< " = " << (socket[0].bytesSent() + socket[1].bytesSent())
			<< std::endl;

	}
}






    std::vector<u64> runCpsiWithPayload(       //by zhangqizhi.zqz
        PRNG& prng,
        std::vector<block>& recvSet,
        std::vector<block>& sendSet,
        u64 nt = 1,
        ValueShareType type = ValueShareType::Xor)
    {
        auto sockets = cp::LocalAsyncSocket::makePair();


        RsCpsiReceiver recver;
        RsCpsiSender sender;

        // auto byteLength = sizeof(block);
        size_t feature_num=128;
        oc::Matrix<u8> senderValues(sendSet.size(), feature_num);

        std::vector<u8> payload(sendSet.size()*feature_num);
        for (size_t i=0; i<sendSet.size()*feature_num; i++)
        {
			if (i%4==0)
			{
				payload[i]=i%10;
			}
			else
			{
				payload[i]=0;
			}
            
        }
        std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
        std::memcpy(senderValues.data(), payload.data(), payload.size());

        recver.init(sendSet.size(), recvSet.size(), feature_num, 40, prng.get(), nt, type);
        sender.init(sendSet.size(), recvSet.size(), feature_num, 40, prng.get(), nt, type);
std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
        RsCpsiReceiver::Sharing rShare;
        RsCpsiSender::Sharing sShare;
std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
        auto p0 = recver.receive(recvSet, rShare, sockets[0]);
        auto p1 = sender.send(sendSet, senderValues, sShare, sockets[1]);
std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
        eval(p0, p1);
        std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
        bool failed = false;
        std::vector<u64> intersection;
        for (u64 i = 0; i < recvSet.size(); ++i)
        {
            auto k = rShare.mMapping[i];

            if (rShare.mFlagBits[k] ^ sShare.mFlagBits[k])
            {
                intersection.push_back(i);
				if  (type == ValueShareType::Xor)
				{
					for (u64 j = 0; j < feature_num; ++j)
                    {
                        u8 rv = rShare.mValues(k, j);
                        u8 sv = sShare.mValues(k, j);
                        std::cout<<(sv ^ rv)<<",";
                    }
                    std::cout<<std::endl;
				}
                else
                {

                    for (u64 j = 0; j < feature_num; j+=4)
                    {
						uint32_t rv = *(uint32_t*)&rShare.mValues(k, j);
						uint32_t sv = *(uint32_t*)&sShare.mValues(k, j);
                        std::cout<<(sv + rv)<<",";
                    }
                    std::cout<<std::endl;
                }
            }
        }


        return intersection;
    }



// void Cpsi_Rs_full_test(const CLP& cmd)
void Cpsi_Rs_full_test()
{
    // u64 n = cmd.getOr("n", 243);
	u64 n = 243;
    std::vector<block> recvSet(n), sendSet(n);
    PRNG prng(ZeroBlock);
    prng.get(recvSet.data(), recvSet.size());
    sendSet = recvSet;

    std::set<u64> exp;
    for (u64 i = 0; i < n; ++i)
        exp.insert(i);

    // auto inter = runCpsi(prng, recvSet, sendSet);
    auto inter = runCpsiWithPayload(prng, recvSet, sendSet, 1, ValueShareType::add32);
    std::set<u64> act(inter.begin(), inter.end());
    if (act != exp)
        throw RTE_LOC;
}





 void runCpsiWithPayload2Party(       //by zhangqizhi.zqz
        // string file_name,
        size_t party_id,
        ValueShareType type = ValueShareType::Xor)
    {
        // auto sockets = cp::LocalAsyncSocket::makePair();
        size_t server_size = 1<<14;
        size_t client_size = 1024;
        size_t feature_num=128;  // int64
		PRNG prng(ZeroBlock);
        coproto::AsioSocket sock;
        if (party_id==0)
        {
			sock = coproto::asioConnect("localhost:6543", true);
            RsCpsiSender sender;
            std::vector<block> sendSet(server_size);
            oc::Matrix<u8> senderValues(server_size, feature_num*8);
            std::vector<int64_t> payload(server_size*feature_num);


            for (size_t i=0; i<server_size; i++)
            {
                sendSet[i] = (block)i;
            }
            for (size_t i=0; i<payload.size(); i++)
            {
                    payload[i]=i%10;     
            }
            std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
            std::memcpy(senderValues.data(), payload.data(), payload.size());
            sender.init(server_size, client_size, feature_num*8, 40, prng.get(), 1, type);
            std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
            RsCpsiSender::Sharing sShare;
            auto p0 = sender.send(sendSet, senderValues, sShare, sock);
			std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
			macoro::sync_wait(p0);
			std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
        }

        if (party_id==1)
        {
			sock = coproto::asioConnect("localhost:6543", false);
            std::vector<block> recvSet(client_size);
            for (size_t i=0; i<client_size; i++)
            {
                recvSet[i] = (block)i;
            }

            RsCpsiReceiver recver;
            recver.init(server_size, client_size, feature_num*8, 40, prng.get(), 1, type);
            RsCpsiReceiver::Sharing rShare;
            auto p1 = recver.receive(recvSet, rShare, sock);
			macoro::sync_wait(p1);
			std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
        }
        
    }






 void runCpsiWithPayload2Party(       //by zhangqizhi.zqz
        // string file_name,
        size_t party_id,
        size_t server_size,
        size_t client_size,
        size_t feature_num,
        ValueShareType type = ValueShareType::Xor)
    {
        // auto sockets = cp::LocalAsyncSocket::makePair();
        // size_t server_size = 1UL<<log_server_size;
        // size_t client_size = 1024;
        // size_t feature_num=128;  // int64
		PRNG prng(ZeroBlock);
        coproto::AsioSocket sock;
        if (party_id==0)
        {
			sock = coproto::asioConnect("localhost:6543", true);
            RsCpsiSender sender;
            std::vector<block> sendSet(server_size);
            oc::Matrix<u8> senderValues(server_size, feature_num*8);
            std::vector<int64_t> payload(server_size*feature_num);


            for (size_t i=0; i<server_size; i++)
            {
                sendSet[i] = (block)i;
            }
            for (size_t i=0; i<payload.size(); i++)
            {
                    payload[i]=i%10;     
            }
            std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
            std::memcpy(senderValues.data(), payload.data(), payload.size());
            sender.init(server_size, client_size, feature_num*8, 40, prng.get(), 1, type);
            std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
            RsCpsiSender::Sharing sShare;
            auto p0 = sender.send(sendSet, senderValues, sShare, sock);
			std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
			macoro::sync_wait(p0);
			std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
        }

        if (party_id==1)
        {
			sock = coproto::asioConnect("localhost:6543", false);
            std::vector<block> recvSet(client_size);
            for (size_t i=0; i<client_size; i++)
            {
                recvSet[i] = (block)i;
            }

            RsCpsiReceiver recver;
            recver.init(server_size, client_size, feature_num*8, 40, prng.get(), 1, type);
            RsCpsiReceiver::Sharing rShare;
            auto p1 = recver.receive(recvSet, rShare, sock);
			macoro::sync_wait(p1);
			std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
        }
        std::cout<<"OK"<<std::endl;
        
    }








void runCpsiWithPayload2PartyIO(       //by zhangqizhi.zqz
        string file_name,
        size_t party_id,
        string out_file_name,
        ValueShareType type = ValueShareType::Xor)
    {
        // auto sockets = cp::LocalAsyncSocket::makePair();
        size_t server_size; // = 1<<14;
        size_t client_size; // = 1024;
        size_t feature_num; //=128;  // int64
		// PRNG prng(ZeroBlock);
        coproto::AsioSocket  sock;
        std::cout<<"file_name="<<file_name<<std::endl;
        oc::Timer timer;
        auto start = timer.setTimePoint("start");
        
        if (party_id==0)
        {
            //------------load data------------------
            std::vector<int64_t> payload;
            std::vector<int64_t> server_ids;
            read_1row(file_name, server_ids, payload);
            server_size = server_ids.size();
            feature_num = payload.size()/server_size;

            // 这个程序需要对样本数做出限制，否则会报错
            // /root/zqz/iPrivJoin/paper_exp_volepsi/cpsi_test.cpp:277
// Assertion `data.size() < std::numeric_limits<u32>::max()` failed.
//   src location: /root/zqz/iPrivJoin/out/install/linux/include/coproto/Socket/SocketScheduler.h:981
//   function: coproto::internal::SockScheduler::makeSendTask(Sock*) [with Sock = coproto::detail::AsioSocket<boost::asio::basic_stream_socket<boost::asio::ip::tcp> >::Sock; macoro::eager_task<void> = macoro::task<void, false>]::<lambda(macoro::FrameBase<macoro::detail::task_promise<void, false> >*)> mutable

            // size_t size_limit = std::numeric_limits<u32>::max()/feature_num/8;
            // server_size = size_limit;
            std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
            std::vector<block> sendSet(server_size);
            oc::Matrix<u8> senderValues(server_size, feature_num*8);
            // std::memcpy(senderValues.data(), payload.data(), payload.size()*8);
            std::memcpy(senderValues.data(), payload.data(), server_size*feature_num*8);

            for (size_t i=0; i<server_size; i++)
            {
                sendSet[i] = (block)server_ids[i];
            }


            //---------------create socket
			sock = coproto::asioConnect("localhost:6543", true);
            send_socket<size_t>(server_size, sock);
            send_socket<size_t>(feature_num, sock);
            recv_socket<size_t>(client_size, sock);



            RsCpsiSender sender;
            

            sender.init(server_size, client_size, feature_num*8, 40, ZeroBlock, 1, type);
            // std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
            RsCpsiSender::Sharing sShare;
            auto p0 = sender.send(sendSet, senderValues, sShare, sock);
			// std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
			macoro::sync_wait(p0);
			// std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
            std::vector<bool> result_labels(sShare.mValues.rows());
            save_csv(sShare.mValues, result_labels, sShare.mFlagBits, out_file_name);
        }

        if (party_id==1)
        {
            //------------load data------------------
            std::vector<int64_t> client_ids;
            std::vector<bool> labels, result_labels;
            read_label_client(file_name, client_ids, labels);
            client_size = client_ids.size();
            std::vector<block> recvSet(client_size);
            for (size_t i=0; i<client_size; i++)
            {
                recvSet[i] = (block) client_ids[i];
            }
            //-------------------create socket ---------------------
			sock = coproto::asioConnect("localhost:6543", false);
            recv_socket<size_t>(server_size, sock);
            recv_socket<size_t>(feature_num, sock);
            send_socket<size_t>(client_size, sock);

            RsCpsiReceiver recver;
            recver.init(server_size, client_size, feature_num*8, 40, ZeroBlock, 1, type);
            RsCpsiReceiver::Sharing rShare;
            auto p1 = recver.receive(recvSet, rShare, sock);
			macoro::sync_wait(p1);
			std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
            std::cout<<"recvSet.size()="<<recvSet.size()<<std::endl;
            result_labels.resize(rShare.mValues.rows());
            for (u64 i = 0; i < recvSet.size(); ++i)
            {
                auto k = rShare.mMapping[i];
                result_labels[k] = labels[i];
                
            }
            // std::cout<<"result_labels=";
            // for (size_t i=0; i<result_labels.size(); i++)
            // {
            //     std::cout<<result_labels[i];
            // }
            // std::cout<<std::endl;
            // std::cout<<rShare.mFlagBits.hex()<<std::endl;
            save_csv(rShare.mValues, result_labels, rShare.mFlagBits, out_file_name);
        }

        float mem_usage = GetMemoryUsage();
        std::cout<<"mem_usage of party"<<party_id<<" = "<<mem_usage<<"MB"<<std::endl;
        std::cout<<"party"<<party_id<<" send "<<sock.bytesSent()/1024.0/1024.0<<"MB"<<std::endl;
        std::cout<<"party"<<party_id<<" receive "<<sock.bytesReceived()/1024.0/1024.0<<"MB"<<std::endl;
        auto end = timer.setTimePoint("end");
        auto tt = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / double(1000);
	    std::cout << "running time of party"<<party_id<<" = " << tt << "s"<<std::endl;
        
    }




void runCpsiWithPayload2PartyIOBatchs(       //by zhangqizhi.zqz
        string file_name,
        size_t party_id,
        string out_file_name,
        size_t server_size_limit,
        ValueShareType type = ValueShareType::Xor)
    {
        // auto sockets = cp::LocalAsyncSocket::makePair();
        size_t server_size; // = 1<<14;
        size_t client_size; // = 1024;
        size_t feature_num; //=128;  // int64
		// PRNG prng(ZeroBlock);
        coproto::AsioSocket  sock;
        std::cout<<"file_name="<<file_name<<std::endl;
        std::cout<<"output_file_name="<<out_file_name<<std::endl;
        oc::Timer timer;
        auto start = timer.setTimePoint("start");
        
        std::ofstream outputFile(out_file_name);
        if (party_id==0)
        {
            //------------load data------------------
            std::vector<int64_t> payload;
            std::vector<int64_t> server_ids;
            read_1row(file_name, server_ids, payload);
            server_size = server_ids.size();
            feature_num = payload.size()/server_size;

            //---------------create socket
            sock = coproto::asioConnect("localhost:6543", true);
            send_socket<size_t>(server_size, sock);
            send_socket<size_t>(feature_num, sock);
            recv_socket<size_t>(client_size, sock);

            // 这个程序需要对样本数做出限制，否则会报错
            // /root/zqz/iPrivJoin/paper_exp_volepsi/cpsi_test.cpp:277
// Assertion `data.size() < std::numeric_limits<u32>::max()` failed.
//   src location: /root/zqz/iPrivJoin/out/install/linux/include/coproto/Socket/SocketScheduler.h:981
//   function: coproto::internal::SockScheduler::makeSendTask(Sock*) [with Sock = coproto::detail::AsioSocket<boost::asio::basic_stream_socket<boost::asio::ip::tcp> >::Sock; macoro::eager_task<void> = macoro::task<void, false>]::<lambda(macoro::FrameBase<macoro::detail::task_promise<void, false> >*)> mutable

            // size_t server_size_limit = std::numeric_limits<u32>::max()/feature_num/8;
            // std::cout<<__FILE__<<":"<<__LINE__<<std::endl;

        
            auto lambda = [&](size_t _server_size, int64_t* _server_ids, int64_t* _payload) {
                // std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
                std::vector<block> sendSet(_server_size);
                // std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
                oc::Matrix<u8> senderValues(_server_size, feature_num*8);
                // std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
                // std::memcpy(senderValues.data(), payload.data(), payload.size()*8);
                std::memcpy(senderValues.data(), _payload, _server_size*feature_num*8);
// std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
                for (size_t i=0; i<_server_size; i++)
                {
                    sendSet[i] = (block)_server_ids[i];
                }

// std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
                send_socket<size_t>(_server_size, sock);

// std::cout<<__FILE__<<":"<<__LINE__<<std::endl;

                RsCpsiSender sender;
                

                sender.init(_server_size, client_size, feature_num*8, 40, ZeroBlock, 1, type);
                // std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
                RsCpsiSender::Sharing sShare;
                auto p0 = sender.send(sendSet, senderValues, sShare, sock);
                // std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
                macoro::sync_wait(p0);
                // std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
                std::vector<bool> result_labels(sShare.mValues.rows());
                // save_csv(sShare.mValues, result_labels, sShare.mFlagBits, out_file_name);
                save_csv(sShare.mValues, result_labels, sShare.mFlagBits, outputFile);
            };

            size_t batch_id=0;
            for (; batch_id<server_size/server_size_limit; batch_id++)
            {
                std::cout<<"batch_id="<<batch_id<<std::endl;
                lambda(server_size_limit, server_ids.data()+batch_id*server_size_limit, payload.data()+batch_id*server_size_limit*feature_num);
            }
            if (server_size%server_size_limit!=0)
            {
                std::cout<<"batch_id="<<batch_id<<std::endl;
                lambda(server_size%server_size_limit, server_ids.data()+batch_id*server_size_limit, payload.data()+batch_id*server_size_limit*feature_num);
            }

        }

        if (party_id==1)
        {
            //------------load data------------------
            std::vector<int64_t> client_ids;
            std::vector<bool> labels, result_labels;
            read_label_client(file_name, client_ids, labels);
            client_size = client_ids.size();
            std::vector<block> recvSet(client_size);
            for (size_t i=0; i<client_size; i++)
            {
                recvSet[i] = (block) client_ids[i];
            }
            //-------------------create socket ---------------------
			sock = coproto::asioConnect("localhost:6543", false);
            recv_socket<size_t>(server_size, sock);
            recv_socket<size_t>(feature_num, sock);
            send_socket<size_t>(client_size, sock);

            auto lambda = [&]()
            {
                // std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
                size_t _server_size;
                // std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
                recv_socket<size_t>(_server_size, sock);
                // std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
                RsCpsiReceiver recver;
                // std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
                recver.init(_server_size, client_size, feature_num*8, 40, ZeroBlock, 1, type);
                // std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
                RsCpsiReceiver::Sharing rShare;
                auto p1 = recver.receive(recvSet, rShare, sock);
                // std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
                macoro::sync_wait(p1);
                // std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
                std::cout<<"recvSet.size()="<<recvSet.size()<<std::endl;
                result_labels.resize(rShare.mValues.rows());
                for (u64 i = 0; i < recvSet.size(); ++i)
                {
                    auto k = rShare.mMapping[i];
                    result_labels[k] = labels[i];
                    
                }
                // save_csv(rShare.mValues, result_labels, rShare.mFlagBits, out_file_name);
                save_csv(rShare.mValues, result_labels, rShare.mFlagBits, outputFile);
            };

            size_t batch_id=0;
            for (; batch_id<server_size/server_size_limit; batch_id++)
            {
                lambda();
            }
            if (server_size%server_size_limit!=0)
            {
                lambda();
            }
            

        }
        outputFile.close();
        float mem_usage = GetMemoryUsage();
        std::cout<<"mem_usage of party"<<party_id<<" = "<<mem_usage<<"MB"<<std::endl;
        std::cout<<"party"<<party_id<<" send "<<sock.bytesSent()/1024.0/1024.0<<"MB"<<std::endl;
        std::cout<<"party"<<party_id<<" receive "<<sock.bytesReceived()/1024.0/1024.0<<"MB"<<std::endl;
        auto end = timer.setTimePoint("end");
        auto tt = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / double(1000);
	    std::cout << "running time of party"<<party_id<<" = " << tt << "s"<<std::endl;
        
    }




int main(int argc, char** argv)
{
	// CPSI_test();
	// Cpsi_Rs_full_test();
    if (argc==2)
    {
    runCpsiWithPayload2Party(std::stoi(argv[1]));
    }
    // runCpsiWithPayload2Party(std::stoi(argv[1]), std::stoi(argv[2]), std::stoi(argv[3]), std::stoi(argv[4]));
    if (argc==4)
    {
        std::cout<<argv[1]<<","<<argv[2]<<","<<argv[3]<<std::endl;
        runCpsiWithPayload2PartyIO(argv[1], std::stoi(argv[2]), argv[3]);
    }
    if  (argc==5)
    {
        std::cout<<argv[1]<<","<<argv[2]<<","<<argv[3]<<","<<argv[4]<<std::endl;
        runCpsiWithPayload2PartyIOBatchs(argv[1], std::stoi(argv[2]), argv[3], std::stoi(argv[4]));
    }
    return 0;
}