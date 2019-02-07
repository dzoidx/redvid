#include <iostream>
#include "net/net.h"
#include "RESP/Encoder.h"
#include "RESP/Decoder.h"
#include <memory>
#include <string>
#pragma comment(lib, "Ws2_32.lib")

int main()
{
    net_init();

    auto e = Encoder();
    e.write_array(1);
    e.write_bulk_string("info", 4);
    auto data = e.to_string();
    auto h = net_create_tcp_socket("localhost", 6379);
    auto r = net_send(h, data.c_str(), data.size());
    std::cout << "Sent bytes " << r << std::endl;
    std::cout << "Shutdown send " << net_shutdown_send(h) << std::endl;
    std::shared_ptr<char> buffer_ptr;
    buffer_ptr.reset(new char[512]);
    auto buffer = buffer_ptr.get();
    std::stringstream result;
    do
    {
        r = net_receive(h, buffer, 511);
        buffer[r] = '\0';
        std::cout << "Received bytes " << r << std::endl;
        result << buffer;
    } while (r > 0);
    auto d = Decoder(result.str().c_str());
    int str_size;
    auto str_raw = d.read_bulk_string(str_size);
    auto str = std::string(str_raw.get(), str_size);
    std::cout << str << std::endl;
    net_close(h);
    net_deinit();
    return 0;
}