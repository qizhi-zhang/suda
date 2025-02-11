#include "RsOpprf.h"


namespace volePSI
{
	void RsOpprfSender::sendLargeMatrix(const osuCrypto::MatrixView<uint64_t> &T, Socket& chl)  // zhangqizhi.zqz
    {
        // First send the dimensions
        // uint64_t rows = T.rows();
        // uint64_t cols = T.cols();
        // macoro::sync_wait(chl.send(rows));
        // macoro::sync_wait(chl.send(cols));

        // Calculate how many elements per chunk we can send
        constexpr size_t chunkSize = size_t(1) << 28; // For example, 268435456 bytes (~256 MB)
        const size_t elementsPerChunk = chunkSize / sizeof(uint64_t);

        uint64_t totalSize = static_cast<uint64_t>(T.size());
        uint64_t offset = 0;
std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
        // Send data in chunks
        while (totalSize > 0) {
			std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
        uint64_t sendCount = std::min<uint64_t>(totalSize, elementsPerChunk);
        oc::span<const uint64_t> keySpan(T.data() + offset, sendCount);

        // Send this chunk
        auto p = chl.send(keySpan);
        macoro::sync_wait(p);
std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
        // Advance offset and decrease the remaining size
        offset += sendCount;
        totalSize -= sendCount;
        }

        // Flush ensures all data is sent out
        macoro::sync_wait(chl.flush());
    }




// void RsOpprfSender::sendLargeMatrix(const osuCrypto::MatrixView<u8> &T, Socket& chl)  // zhangqizhi.zqz
//     {
//         // First send the dimensions
//         // uint64_t rows = T.rows();
//         // uint64_t cols = T.cols();
//         // macoro::sync_wait(chl.send(rows));
//         // macoro::sync_wait(chl.send(cols));

//         // Calculate how many elements per chunk we can send
//         constexpr size_t chunkSize = size_t(1) << 28; // For example, 268435456 bytes (~256 MB)
//         const size_t elementsPerChunk = chunkSize / sizeof(uint8_t);

//         uint64_t totalSize = static_cast<uint64_t>(T.size());
//         uint64_t offset = 0;

//         // Send data in chunks
// 		std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
//         while (totalSize > 0) {
// 			std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
//         uint64_t sendCount = std::min<uint64_t>(totalSize, elementsPerChunk);
//         oc::span<const uint8_t> keySpan(T.data() + offset, sendCount);

//         // Send this chunk
//         auto p = chl.send(keySpan);
//         macoro::sync_wait(p);
// 		// MC_AWAIT(chl.send(coproto::copy(keySpan)));
// std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
//         // Advance offset and decrease the remaining size
//         offset += sendCount;
//         totalSize -= sendCount;
// 		std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
//         }
// 		std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
//         // Flush ensures all data is sent out
//         macoro::sync_wait(chl.flush());
//     }




Proto RsOpprfSender::sendLargeMatrix(const osuCrypto::MatrixView<u8> &T, Socket& chl)  // zhangqizhi.zqz
    {
        // First send the dimensions
        // uint64_t rows = T.rows();
        // uint64_t cols = T.cols();
        // macoro::sync_wait(chl.send(rows));
        // macoro::sync_wait(chl.send(cols));

        // Calculate how many elements per chunk we can send
        constexpr size_t chunkSize = size_t(1) << 28; // For example, 268435456 bytes (~256 MB)
        const size_t elementsPerChunk = chunkSize / sizeof(uint8_t);

        uint64_t totalSize = static_cast<uint64_t>(T.size());
        uint64_t offset = 0;

        // Send data in chunks
		std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
        while (totalSize > 0) {
			std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
        uint64_t sendCount = std::min<uint64_t>(totalSize, elementsPerChunk);
        oc::span<const uint8_t> keySpan(T.data() + offset, sendCount);

        // Send this chunk
        // auto p = chl.send(keySpan);
        // macoro::sync_wait(p);

		co_await chl.send(std::move(keySpan));
std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
        // Advance offset and decrease the remaining size
        offset += sendCount;
        totalSize -= sendCount;
		std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
        }
		std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
        // Flush ensures all data is sent out
        // macoro::sync_wait(chl.flush());
    }





    // Define a function to receive a large matrix in chunks
void RsOpprfReceiver::recvLargeMatrix(osuCrypto::Matrix<uint64_t> &T, Socket& chl) //zhangqizhi.zqz
{
    // First receive the dimensions
    // uint64_t rows, cols;
    // macoro::sync_wait(chl.recv(rows));
    // macoro::sync_wait(chl.recv(cols));

    // T.resize(rows, cols);

    // Same chunk logic as sending
    constexpr size_t chunkSize = size_t(1) << 28; // Must match sending side or be known
    const size_t elementsPerChunk = chunkSize / sizeof(uint64_t);

    uint64_t totalSize = static_cast<uint64_t>(T.size());
    uint64_t offset = 0;
	std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
    // Receive data in chunks
    while (totalSize > 0) {
        uint64_t recvCount = std::min<uint64_t>(totalSize, elementsPerChunk);
        oc::span<uint64_t> recvSpan(T.data() + offset, recvCount);

        // Receive this chunk
        macoro::sync_wait(chl.recv(recvSpan));

        offset += recvCount;
        totalSize -= recvCount;
		std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
    }
    macoro::sync_wait(chl.flush());
}




// void RsOpprfReceiver::recvLargeMatrix(osuCrypto::Matrix<u8> &T, Socket& chl) //zhangqizhi.zqz
// {
//     // First receive the dimensions
//     // uint64_t rows, cols;
//     // macoro::sync_wait(chl.recv(rows));
//     // macoro::sync_wait(chl.recv(cols));

//     // T.resize(rows, cols);

//     // Same chunk logic as sending
//     constexpr size_t chunkSize = size_t(1) << 28; // Must match sending side or be known
//     const size_t elementsPerChunk = chunkSize / sizeof(uint8_t);

//     uint64_t totalSize = static_cast<uint64_t>(T.size());
//     uint64_t offset = 0;
// std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
//     // Receive data in chunks
//     while (totalSize > 0) {
// 		std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
//         uint64_t recvCount = std::min<uint64_t>(totalSize, elementsPerChunk);
//         oc::span<uint8_t> recvSpan(T.data() + offset, recvCount);

//         // Receive this chunk
//         macoro::sync_wait(chl.recv(recvSpan));

//         offset += recvCount;
//         totalSize -= recvCount;
// 		std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
//     }
//     macoro::sync_wait(chl.flush());
// }




Proto RsOpprfReceiver::recvLargeMatrix(osuCrypto::Matrix<u8> &T, Socket& chl) //zhangqizhi.zqz
{
    // First receive the dimensions
    // uint64_t rows, cols;
    // macoro::sync_wait(chl.recv(rows));
    // macoro::sync_wait(chl.recv(cols));

    // T.resize(rows, cols);

    // Same chunk logic as sending
    constexpr size_t chunkSize = size_t(1) << 28; // Must match sending side or be known
    const size_t elementsPerChunk = chunkSize / sizeof(uint8_t);

    uint64_t totalSize = static_cast<uint64_t>(T.size());
    uint64_t offset = 0;
std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
    // Receive data in chunks
    while (totalSize > 0) {
		std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
        uint64_t recvCount = std::min<uint64_t>(totalSize, elementsPerChunk);
        oc::span<uint8_t> recvSpan(T.data() + offset, recvCount);

        // Receive this chunk
        // macoro::sync_wait(chl.recv(recvSpan));
		co_await chl.recv(recvSpan);

        offset += recvCount;
        totalSize -= recvCount;
		std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
    }
    // macoro::sync_wait(chl.flush());
}








	void RsOpprfSender::oprfEval(span<const block> X, span<u8> out, span<const u8> add, u64 m, u64 numThreads)
	{
		auto n = X.size();
		auto nm = n * m;

		if (out.size() < nm)
			throw RTE_LOC;

		std::unique_ptr<u8[]> tempPtr;
		span<block> oprfOutput;
		if (m == sizeof(block) && !(out.data() == add.data()))
			oprfOutput = span<block>((block*)out.data(), n);
		else
		{
			tempPtr.reset(new u8[n * sizeof(block)]);
			oprfOutput = span<block>((block*)tempPtr.get(), n);
		}

		mOprfSender.eval(X, oprfOutput, numThreads);

		if (m <= sizeof(block))
		{
			// short string case
			for (u64 i = 0, ij = 0; i < n; ++i)
			{
				for (u64 j = 0; j < m; ++j, ++ij)
				{
					out[ij] = add[ij] ^ oprfOutput[i].get<u8>(j);
				}
			}
		}
		else if (m > sizeof(block))
		{
			// long string case

			std::vector<block> buffer(oc::divCeil(m, sizeof(block)));
			auto buffPtr = (u8*)buffer.data();
			for (u64 i = 0, ij = 0; i < n; ++i)
			{
				oc::AES enc(oprfOutput[i]);
				enc.ecbEncCounterMode(0, buffer);
				for (u64 j = 0; j < m; ++j, ++ij)
					out[ij] = add[ij] ^ buffPtr[j];
			}
		}
	}


	Proto RsOpprfSender::send(u64 recverSize, span<const block> X, MatrixView<u8> val, PRNG& prng, u64 numThreads, Socket& chl)
	{
		auto n = u64{};
		auto m = u64{};
		auto nm = u64{};
		auto hashingSeed = block{};
		auto type = PaxosParam::Binary;
		auto diffPtr = std::unique_ptr<u8[]>{};
		auto diffU8 = span<u8>{};
		std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
		setTimePoint("RsOpprfSender::send begin");
		n = X.size();
		m = val.cols();
		nm = n * m;

		if (val.rows() != n)
			throw RTE_LOC;

		if (!n || !m)
			throw RTE_LOC;
		std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
		hashingSeed = prng.get();
		co_await(chl.send(std::move(hashingSeed)));
		std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
		type = m % sizeof(block) ? PaxosParam::Binary : PaxosParam::GF128;
		mPaxos.init(n, 1 << 14, 3, 40, type, hashingSeed);

		if (mTimer)
			mOprfSender.setTimer(*mTimer);
		std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
		co_await(mOprfSender.send(recverSize, prng, chl, numThreads));
		std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
		diffPtr.reset(new u8[nm]);
		diffU8 = span<u8>(diffPtr.get(), nm);

		if (diffU8.size() < val.size())
			throw RTE_LOC;
		memset(diffU8.data(), 0, val.size());
		oprfEval(X, diffU8, val, m, numThreads);

		mPaxosByteWidth = m;
		mP.resize(mPaxos.size() * m);

		setTimePoint("RsOpprfSender::send eval");
		std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
		if (m == 16)
		{
			// special case for block.
			auto diff = span<block>((block*)diffU8.data(), n);
			auto P = span<block>((block*)mP.data(), mPaxos.size());
		std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
			// double check the bounds.
			if ((u8*)(diff.data() + diff.size()) > diffU8.data() + diffU8.size())
				throw RTE_LOC;
			if ((u8*)(mP.data() + mP.size()) > mP.data() + mP.size())
				throw RTE_LOC;
		std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
			mPaxos.solve<block>(X, diff, P, &prng, numThreads);
					std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
		}
		else
		{
			//throw RTE_LOC;
			auto diff = MatrixView<u8>(diffPtr.get(), n, m);
			auto P = MatrixView<u8>((u8*)mP.data(), mPaxos.size(), m);
		std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
			// double check the bounds.
			if (diff.data() + diff.size() > diffU8.data() + diffU8.size())
				throw RTE_LOC;
			if (mP.data() + mP.size() > mP.data() + mP.size())
				throw RTE_LOC;
		std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
			mPaxos.solve<u8>(X, diff, P, &prng, numThreads);
					std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
		}
		diffPtr.reset();

		setTimePoint("RsOpprfSender::send paxos solve");

		// co_await(chl.send(coproto::copy(mP))); // zhangqizhi.zqz
		std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
		auto P1 = MatrixView<u8>((u8*)mP.data(), mPaxos.size(), m); // zhangqizhi.zqz
		std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
		co_await(sendLargeMatrix(P1, chl));  // zhangqizhi.zqz
		std::cout<<__FILE__<<":"<<__LINE__<<std::endl;


		setTimePoint("RsOpprfSender::send end");

	}

	void RsOpprfSender::eval(span<const block> val, MatrixView<u8> output, u64 numThreads)
	{
		auto m = output.cols();

		if (mPaxosByteWidth != m)
			throw RTE_LOC;

		MatrixView<u8> P(mP.data(), mPaxos.size(), output.cols());

		setTimePoint("RsOpprfSender::eval begin");
		if (m == sizeof(block))
		{
			// spacial case for block
			auto oo = span<block>((block*)output.data(), output.rows());
			auto pp = span<block>((block*)P.data(), P.rows());
			mPaxos.decode<block>(val, oo, pp);
		}
		else
		{
			mPaxos.decode<u8>(val, output, P);
		}

		setTimePoint("RsOpprfSender::eval paxos decode");

		// output +=  oprf(val)
		auto outU8 = span<u8>(output.data(), output.size());
		oprfEval(val, outU8, outU8, m, numThreads);

		setTimePoint("RsOpprfSender::eval end");

	}


	Proto RsOpprfReceiver::receive(
		u64 senderSize,
		span<const block> values,
		MatrixView<u8> outputs,
		PRNG& prng,
		u64 numThreads,
		Socket& chl)
	{
		auto n = u64{0}, m = u64{0};
		auto paxos = Baxos{};
		auto type = PaxosParam::Binary;
		auto temp = BasicVector<block>{};
		auto oprfOutput = span<block>{};
		auto p = Matrix<u8>{ };

		setTimePoint("RsOpprfReceiver::receive begin");

		if (values.size() != outputs.rows())
			throw RTE_LOC;
		n = outputs.rows();
		m = outputs.cols();
std::cout<<__FILE__<<":"<<__LINE__<<std::endl;

		co_await chl.recv(paxos.mSeed);
		type = m % sizeof(block) ? PaxosParam::Binary : PaxosParam::GF128;
		paxos.init(senderSize, 1 << 14, 3, 40, type, paxos.mSeed);
std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
		if (mTimer)
			mOprfReceiver.setTimer(*mTimer);

		if (outputs.cols() >= sizeof(block))
		{
			// reuse memory. extra logic to make sure oprfOutput is properly aligned.
			auto ptr = outputs.data() + outputs.size();
			auto offset = (u64)ptr % sizeof(block);
			if (offset)
				ptr -= offset;
			assert((((u64)ptr) % sizeof(block)) == 0);

			oprfOutput = span<block>((block*)(ptr)-n, n);

			assert(
				(void*)(oprfOutput.data() + oprfOutput.size()) <=
				(void*)(outputs.data() + outputs.size()));
		}
		else
		{
			temp.resize(n);
			oprfOutput = temp;
		}
		std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
		co_await mOprfReceiver.receive(values, oprfOutput, prng, chl, numThreads);

std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
		p.resize(paxos.size(), m, oc::AllocType::Uninitialized);
		std::cout<<__FILE__<<":"<<__LINE__<<std::endl;
		// co_await chl.recv(p);  //zhangqizhi.zqz
		co_await recvLargeMatrix(p, chl); //zhangqizhi.zqz
		setTimePoint("RsOpprfReceiver::receive recv");

		if (m == sizeof(block))
		{
			auto v = span<block>((block*)values.data(), n);
			auto o = span<block>((block*)outputs.data(), n);
			auto pp = span<block>((block*)p.data(), paxos.size());

			paxos.mAddToDecode = true;
			paxos.decode<block>(v, o, pp, numThreads);
		}
		else
		{
			if (m > sizeof(block))
			{

				std::vector<block> buffer(oc::divCeil(m, sizeof(block)));
				auto buffPtr = (u8*)buffer.data();

				for (u64 i = 0; i < n; ++i)
				{
					oc::AES enc(oprfOutput[i]);
					enc.ecbEncCounterMode(0, buffer);
					memcpy(&outputs(i * m), buffPtr, m);
				}
			}
			else
			{
				for (u64 i = 0; i < n; ++i)
					memcpy(&outputs(i * m), &oprfOutput[i], m);
			}

			temp = {};
			paxos.mAddToDecode = true;
			paxos.decode<u8>(values, outputs, p, numThreads);
		}
		temp = {};

		setTimePoint("RsOpprfReceiver::receive end");

		co_return;
	}
}