
#ifndef NETIO
#define NETIO
#include <coproto/Common/macoro.h>
#include <coproto/Socket/AsioSocket.h>
// #include "utils.h"

// using namespace coproto;


//   ---------- send to receive from address -----------


template <typename T>
void send_vector(std::vector<T> &data, std::string  address)
{
    const auto wait_start_time = std::chrono::system_clock::now();

    // auto socket = osuCrypto::cp::asioConnect(context.address, false);

    auto socket = coproto::asioConnect(address, false);

    const auto wait_end_time = std::chrono::system_clock::now();

    const auto wait_duration_time = wait_end_time - wait_start_time;
    // context.timings.wait += wait_duration_time.count();
    auto p = socket.send(data);
    coproto::sync_wait(p);
    coproto::sync_wait(socket.flush());
    socket.close();
    // context.totalReceive += socket.bytesReceived();
    // context.totalSend += socket.bytesSent();
}


template <typename T>
std::vector<T> recv_vector(std::string address, size_t size)
{
    const auto wait_start_time = std::chrono::system_clock::now();

    auto socket = coproto::asioConnect(address, true);

    const auto wait_end_time = std::chrono::system_clock::now();

    // const duration_millis wait_duration_time = wait_end_time - wait_start_time;
    // context.timings.wait += wait_duration_time.count();

    std::vector<uint64_t> data(size);
    auto p = socket.recv(data);
    coproto::sync_wait(p);
    coproto::sync_wait(socket.flush());
    socket.close();
    // context.totalReceive += socket.bytesReceived();
    // context.totalSend += socket.bytesSent();
    return data;
}




template <typename T>
void send_vector_auto(std::vector<T> &data, std::string  address)
{
    const auto wait_start_time = std::chrono::system_clock::now();

    // auto socket = osuCrypto::cp::asioConnect(context.address, false);

    auto socket = coproto::asioConnect(address, false);

    const auto wait_end_time = std::chrono::system_clock::now();

    const auto wait_duration_time = wait_end_time - wait_start_time;
    // context.timings.wait += wait_duration_time.count();
    auto p = socket.send(data.size());
    coproto::sync_wait(p);
    auto q = socket.send(data);
    coproto::sync_wait(q);
    coproto::sync_wait(socket.flush());
    socket.close();
    // context.totalReceive += socket.bytesReceived();
    // context.totalSend += socket.bytesSent();
}


template <typename T>
std::vector<T> recv_vector_auto(std::string address)
{
    const auto wait_start_time = std::chrono::system_clock::now();

    auto socket = coproto::asioConnect(address, true);

    const auto wait_end_time = std::chrono::system_clock::now();

    // const duration_millis wait_duration_time = wait_end_time - wait_start_time;
    // context.timings.wait += wait_duration_time.count();
    size_t size;
    auto p = socket.recv(size);
    coproto::sync_wait(p);
    std::vector<T> data(size);
    auto q = socket.recv(data);
    coproto::sync_wait(q);
    coproto::sync_wait(socket.flush());
    socket.close();
    // context.totalReceive += socket.bytesReceived();
    // context.totalSend += socket.bytesSent();
    return data;
}







template <typename T>
void send(T & data, std::string  address)
{
    const auto wait_start_time = std::chrono::system_clock::now();

    // auto socket = osuCrypto::cp::asioConnect(context.address, false);

    auto socket = coproto::asioConnect(address, false);

    const auto wait_end_time = std::chrono::system_clock::now();

    const auto wait_duration_time = wait_end_time - wait_start_time;
    // context.timings.wait += wait_duration_time.count();
    auto p = socket.send(data);
    coproto::sync_wait(p);
    coproto::sync_wait(socket.flush());
    socket.close();
    // context.totalReceive += socket.bytesReceived();
    // context.totalSend += socket.bytesSent();
}

template <typename T>
T recv(std::string address)
{
    const auto wait_start_time = std::chrono::system_clock::now();

    auto socket = coproto::asioConnect(address, true);

    const auto wait_end_time = std::chrono::system_clock::now();

    // const duration_millis wait_duration_time = wait_end_time - wait_start_time;
    // context.timings.wait += wait_duration_time.count();

    T data;
    auto q = socket.recv(data);
    coproto::sync_wait(q);
    coproto::sync_wait(socket.flush());
    socket.close();
    return data;
}


template <typename T>
void recv(T & data, std::string address)
{
    const auto wait_start_time = std::chrono::system_clock::now();

    auto socket = coproto::asioConnect(address, true);

    const auto wait_end_time = std::chrono::system_clock::now();

    // const duration_millis wait_duration_time = wait_end_time - wait_start_time;
    // context.timings.wait += wait_duration_time.count();

    auto q = socket.recv(data);
    coproto::sync_wait(q);
    coproto::sync_wait(socket.flush());
    socket.close();
}


template <typename T>
T recv_auto(std::string address)
{
    const auto wait_start_time = std::chrono::system_clock::now();

    auto socket = coproto::asioConnect(address, true);

    const auto wait_end_time = std::chrono::system_clock::now();

    // const duration_millis wait_duration_time = wait_end_time - wait_start_time;
    // context.timings.wait += wait_duration_time.count();

    T data;
    auto q =  socket.recvResize(data);
    coproto::sync_wait(q);
    coproto::sync_wait(socket.flush());
    socket.close();
    return data;
}
     




//----- send to receive from socket ------------------






template <typename T>
void send(T & data, coproto::Socket & socket)
{
    auto p = socket.send(data);
    coproto::sync_wait(p);
    coproto::sync_wait(socket.flush());
    socket.close();

}

template <typename T>
void recv(T & data, coproto::Socket & socket)
{
    auto q = socket.recv(data);
    coproto::sync_wait(q);
    coproto::sync_wait(socket.flush());
    socket.close();
}









template <typename T>
void send_vector_socket(std::vector<T> &data, coproto::Socket & socket)
{
    const auto wait_start_time = std::chrono::system_clock::now();

    // auto socket = osuCrypto::cp::asioConnect(context.address, false);

    // auto socket = coproto::asioConnect(address, false);

    const auto wait_end_time = std::chrono::system_clock::now();

    const auto wait_duration_time = wait_end_time - wait_start_time;
    // context.timings.wait += wait_duration_time.count();
    auto p = socket.send(data);
    coproto::sync_wait(p);
    coproto::sync_wait(socket.flush());
    // socket.close();
    // context.totalReceive += socket.bytesReceived();
    // context.totalSend += socket.bytesSent();
}


template <typename T>
std::vector<T> recv_vector_socket(coproto::Socket & socket, size_t size)
{
    const auto wait_start_time = std::chrono::system_clock::now();

    // auto socket = coproto::asioConnect(address, true);

    const auto wait_end_time = std::chrono::system_clock::now();

    // const duration_millis wait_duration_time = wait_end_time - wait_start_time;
    // context.timings.wait += wait_duration_time.count();

    std::vector<uint64_t> data(size);
    auto p = socket.recv(data);
    coproto::sync_wait(p);
    coproto::sync_wait(socket.flush());
    // socket.close();
    // context.totalReceive += socket.bytesReceived();
    // context.totalSend += socket.bytesSent();
    return data;
}



template <typename T>
void send_vector_auto_socket(std::vector<T> &data, coproto::Socket & socket)
{
    const auto wait_start_time = std::chrono::system_clock::now();

    // auto socket = osuCrypto::cp::asioConnect(context.address, false);

    // auto socket = coproto::asioConnect(address, false);

    const auto wait_end_time = std::chrono::system_clock::now();

    const auto wait_duration_time = wait_end_time - wait_start_time;
    // context.timings.wait += wait_duration_time.count();
    auto p = socket.send(data.size());
    coproto::sync_wait(p);
    auto q = socket.send(data);
    coproto::sync_wait(q);
    coproto::sync_wait(socket.flush());
    // socket.close();
    // context.totalReceive += socket.bytesReceived();
    // context.totalSend += socket.bytesSent();
}


template <typename T>
std::vector<T> recv_vector_auto_socket(coproto::Socket & socket)
{
    const auto wait_start_time = std::chrono::system_clock::now();

    // auto socket = coproto::asioConnect(address, true);

    const auto wait_end_time = std::chrono::system_clock::now();

    // const duration_millis wait_duration_time = wait_end_time - wait_start_time;
    // context.timings.wait += wait_duration_time.count();
    size_t size;
    auto p = socket.recv(size);
    coproto::sync_wait(p);
    std::vector<T> data(size);
    auto q = socket.recv(data);
    coproto::sync_wait(q);
    coproto::sync_wait(socket.flush());
    // socket.close();
    // context.totalReceive += socket.bytesReceived();
    // context.totalSend += socket.bytesSent();
    return data;
}







template <typename T>
void send_socket(T & data, coproto::Socket & socket)
{
    const auto wait_start_time = std::chrono::system_clock::now();

    // auto socket = osuCrypto::cp::asioConnect(context.address, false);

    // auto socket = coproto::asioConnect(address, false);

    const auto wait_end_time = std::chrono::system_clock::now();

    const auto wait_duration_time = wait_end_time - wait_start_time;
    // context.timings.wait += wait_duration_time.count();
    auto p = socket.send(data);
    coproto::sync_wait(p);
    coproto::sync_wait(socket.flush());
    // socket.close();
    // context.totalReceive += socket.bytesReceived();
    // context.totalSend += socket.bytesSent();
}

template <typename T>
T recv_socket(coproto::Socket & socket)
{
    const auto wait_start_time = std::chrono::system_clock::now();

    // auto socket = coproto::asioConnect(address, true);

    const auto wait_end_time = std::chrono::system_clock::now();

    // const duration_millis wait_duration_time = wait_end_time - wait_start_time;
    // context.timings.wait += wait_duration_time.count();

    T data;
    auto q = socket.recv(data);
    coproto::sync_wait(q);
    coproto::sync_wait(socket.flush());
    // socket.close();
    return data;
}



template <typename T>
void recv_socket(T& data, coproto::Socket & socket)
{
    const auto wait_start_time = std::chrono::system_clock::now();

    // auto socket = coproto::asioConnect(address, true);

    const auto wait_end_time = std::chrono::system_clock::now();

    // const duration_millis wait_duration_time = wait_end_time - wait_start_time;
    // context.timings.wait += wait_duration_time.count();

    auto q = socket.recv(data);
    coproto::sync_wait(q);
    coproto::sync_wait(socket.flush());
    // socket.close();
}





template <typename T>
T recv_auto_socket(coproto::Socket & socket)
{
    const auto wait_start_time = std::chrono::system_clock::now();

    // auto socket = coproto::asioConnect(address, true);

    const auto wait_end_time = std::chrono::system_clock::now();

    // const duration_millis wait_duration_time = wait_end_time - wait_start_time;
    // context.timings.wait += wait_duration_time.count();

    T data;
    auto q =  socket.recvResize(data);
    coproto::sync_wait(q);
    coproto::sync_wait(socket.flush());
    // socket.close();
    return data;
}
     









void send_stringstream_socket(stringstream & data, coproto::Socket & socket)
{
    string data_str = data.str();
    send_socket<string>(data_str, socket);
}

stringstream recv_stringstream_socket(coproto::Socket & socket)
{
    string r_str = recv_auto_socket<string>(socket);
    stringstream r;
    r << r_str;
    return r;
}






#endif