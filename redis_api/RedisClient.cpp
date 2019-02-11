//
// Created by morgan on 10.02.2019.
//

#include <memory>
#include "RedisClient.h"
#include "../net/net.h"
#include "../RESP/Encoder.h"

std::string request(Encoder& encoded_data, const char* host, int port);

RedisClient::RedisClient(const char* host, int port)
: host_(host), port_(port)
{
    auto r = info();
    auto sect = r.get(std::string("Server"));
    auto ver = sect.get("redis_version");
    version_ = RedisVersion(ver);
}

Info RedisClient::info()
{
    auto e = Encoder();
    e.write_array(1);
    e.write_bulk_string("info", 4);
    auto resp = request(e, host_.c_str(), port_);
    auto d = Decoder(resp.c_str());
    auto info = Info(d);
    return info;
}

bool RedisClient::select(int db_index)
{
    auto e= Encoder();
    std::stringstream ss_num;
    ss_num << db_index;
    auto num = ss_num.str();
    e.write_array(2);
    e.write_bulk_string("select", 6);
    e.write_bulk_string(num.c_str(), num.size());
    auto resp = request(e, host_.c_str(), port_);
    auto result = resp.c_str();
    auto d = Decoder(result);
    if(d.peek_next() == DataType::SimpleString)
    {
        auto r = d.read_simple_string();
        return r == "OK";
    }
    return false;
}

std::string request(Encoder& encoded_data, const char* host, int port)
{
    auto data = encoded_data.to_string();
    auto s = net_create_tcp_socket(host, port);
    net_send(s, data.c_str(), data.size());
    std::shared_ptr<char> buffer_ptr;
    buffer_ptr.reset(new char[512]);
    auto buffer = buffer_ptr.get();
    std::stringstream result;
    int r, t = 0;
    do
    {
        r = net_receive(s, buffer, 511);
        buffer[r] = '\0';
        if(t == 0)
        {
            net_shutdown_send(s);
        }
        t += r;
        result << buffer;
    } while (r > 0);
    net_close(s);
    return result.str();
}