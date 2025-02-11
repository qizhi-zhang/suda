#include "ggm.h"
#include <bitset>
#include <cassert>
#include <coproto/Socket/AsioSocket.h>
#include <cryptoTools/Common/block.h>
#include <cryptoTools/Crypto/PRNG.h>
#include <cstdint>
#include <iostream>
#include <libOTe/Base/BaseOT.h>
#include <string>
#include <utility>
#include <vector>
#include "joinData.h"

std::string convertChosenbits(const uint64_t &num, const uint64_t &l)
{
    return std::bitset<64>(num).to_string().substr(64 - l);
}

uint64_t calculateIndex(const std::string &path)
{
    uint64_t l = path.length();
    uint64_t index = 0;

    for (auto i : path.substr(0, l - 1)) {
        index = index * 2 + (i == '0' ? 2 : 1);
    }
    index = index * 2 + (path[l - 1] != '0' ? 2 : 1);
    return index;
}

GGMTree::GGMTree(const uint64_t &high) : node((1 << high) - 1, { 0, 0 }), high(high)
{}

GGMTree::GGMTree(const uint64_t &high, const block key)
    : node((1 << high) - 1, { 0, 0 }), high(high)
{
    set(0, std::move(key));
}

GGMTree::GGMTree(
    const uint64_t &high, const std::vector<osuCrypto::block> &received, const std::string &choices)
    : node((1 << high) - 1), high(high)
{
    assert(choices.length() == high - 1 && received.size() == high - 1);
    for (uint64_t i = 1; i < high; i++) {
        auto index = calculateIndex(choices.substr(0, i));
        if (choices[i - 1] == '0') {
            set(index, received[i - 1] ^ xorLeft(i));
        } else {
            set(index, received[i - 1] ^ xorRight(i));
        }
    }
}

void GGMTree::set(const uint64_t &index, const block key)
{
    if (index >= node.size())
        return;

    std::vector<block> dest(2);
    osuCrypto::PRNG prng(key);
    prng.get<block>(dest.data(), 2);
    node.at(index) = std::move(key);
    set(index * 2 + 1, std::move(dest[0]));
    set(index * 2 + 2, std::move(dest[1]));
}

block GGMTree::xorLeft(const uint64_t &layer) const
{
    block result({ 0, 0 });
    for (uint64_t i = (1 << layer) - 1; i < (1 << (layer + 1)) - 1; i += 2) {
        result ^= node.at(i);
    }
    return result;
}

block GGMTree::xorRight(const uint64_t &layer) const
{
    block result({ 0, 0 });
    for (uint64_t i = 1 << layer; i < (1 << (layer + 1)) - 1; i += 2) {
        result ^= node.at(i);
    }
    return result;
}

block GGMTree::xorLayer(const uint64_t &layer) const
{
    block result({ 0, 0 });
    for (uint64_t i = (1 << layer) - 1; i < (1 << (layer + 1)) - 1; i += 1) {
        result ^= node.at(i);
    }
    return result;
}

std::vector<std::array<osuCrypto::block, 2>> GGMTree::eval() const
{
    std::vector<std::array<osuCrypto::block, 2>> result(high - 1);
    for (uint64_t i = 1; i < high; i++) {
        result[i - 1][0] = xorLeft(i);
        result[i - 1][1] = xorRight(i);
    }
    return result;
}

std::vector<block> GGMTree::leaf() const
{
    const uint64_t index = (1 << (high - 1)) - 1;
    return std::vector<block>(node.begin() + index, node.end());
}

void ggm_send(const GGMTree &g, const std::string &address, PsiAnalyticsContext &context)
{
    const auto wait_start_time = std::chrono::system_clock::now();
    coproto::Socket chl = coproto::asioConnect(address, true);
    const auto wait_end_time = std::chrono::system_clock::now();
    const duration_millis wait_time = wait_end_time - wait_start_time;
    context.timings.wait += wait_time.count();
    osuCrypto::DefaultBaseOT baseOTs;
    osuCrypto::PRNG prng(osuCrypto::block(4234335, 3445235));
    auto message = g.eval();
    coproto::sync_wait(baseOTs.sendChosen(message, prng, chl));
    osuCrypto::cp::sync_wait(chl.flush());
    context.totalReceive += chl.bytesReceived();
    context.totalSend += chl.bytesSent();
    chl.close();
}

GGMTree ggm_recv(
    const uint64_t &path_num,
    const uint64_t &high,
    const std::string &address,
    PsiAnalyticsContext &context)
{
    auto chosen_bits = convertChosenbits(~path_num, high - 1);
    const auto wait_start_time = std::chrono::system_clock::now();
    coproto::Socket chl = coproto::asioConnect(address, false);
    const auto wait_end_time = std::chrono::system_clock::now();
    const duration_millis wait_time = wait_end_time - wait_start_time;
    context.timings.wait += wait_time.count();
    osuCrypto::DefaultBaseOT baseOTs;
    osuCrypto::PRNG prng(osuCrypto::block(4234335, 3445235));
    std::vector<osuCrypto::block> received(high - 1);
    coproto::sync_wait(baseOTs.receiveChosen(chosen_bits, received, prng, chl));
    osuCrypto::cp::sync_wait(chl.flush());
    context.totalReceive += chl.bytesReceived();
    context.totalSend += chl.bytesSent();
    chl.close();
    return (GGMTree(high, received, chosen_bits));
}

void GGMTree::print() const
{
    for (uint64_t i = 1; i <= high; i++) {
        for (uint64_t j = (1 << (i - 1)) - 1; j < (1 << i) - 1; j++) {
            std::cout << node.at(j) << " ";
        }
        std::cout << std::endl;
    }
}

void printV(const std::vector<uint64_t> &v)
{
    for (auto i : v) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}