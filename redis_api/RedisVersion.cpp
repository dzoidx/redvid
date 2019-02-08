//
// Created by morgan on 08.02.2019.
//

#include "RedisVersion.h"

RedisVersion::RedisVersion(std::string version)
:version_()
{
    auto pos = 0;
    auto major_dot = version.find('.');
    auto minor_dot = version.find('.', major_dot + 1);
    unsigned major = 0;
    unsigned minor = 0;
    unsigned revision = 0;

    if(major_dot != std::string::npos)
    {
        auto s = version.substr(0, major_dot);
        major = atoi(s.c_str());
        pos = major_dot + 1;
    }

    if(minor_dot != std::string::npos)
    {
        auto s = version.substr(pos, minor_dot);
        minor = atoi(s.c_str());
        pos = minor_dot + 1;
    }

    if(pos == 0)
    {
        version_ = atoi(version.c_str());
        return;
    }
    {
        auto s = version.substr(pos);
        revision = atoi(s.c_str());
    }
    init(major, minor, revision);
}