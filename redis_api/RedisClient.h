//
// Created by morgan on 10.02.2019.
//

#ifndef REDVID_REDISCLIENT_H
#define REDVID_REDISCLIENT_H

#include "RedisVersion.h"
#include <string>
#include "Info.h"

class RedisClient
{
public:
    RedisClient(const char* host, int port);
    RedisVersion get_version();
    Info info();
private:
    RedisVersion version_;
    std::string host_;
    int port_;
};


#endif //REDVID_REDISCLIENT_H
