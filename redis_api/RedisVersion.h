//
// Created by morgan on 08.02.2019.
//

#ifndef REDVID_REDISVERSION_H
#define REDVID_REDISVERSION_H

#include <string>

class RedisVersion
{
public:
    RedisVersion(std::string version);
    constexpr RedisVersion(unsigned major, unsigned minor = 0, unsigned revision = 0):version_{} { init(major, minor, revision); }

    inline bool operator >(const RedisVersion& sec) const { return version_ > sec.version_; }
    inline bool operator >=(const RedisVersion& sec) const { return version_ >= sec.version_; }
    inline bool operator <(const RedisVersion& sec) const { return version_ < sec.version_; }
    inline bool operator <=(const RedisVersion& sec) const { return version_ <= sec.version_; }
    inline bool operator ==(const RedisVersion& sec) const { return version_ == sec.version_; }
    inline bool operator !=(const RedisVersion& sec) const { return version_ != sec.version_; }

    operator int() { return version_; }

private:
    inline constexpr void init(unsigned major, unsigned minor, unsigned revision) {version_ = (major << 24) | (minor << 12) | (revision & 0xfff);}
private:
    unsigned int version_;
};

#endif //REDVID_REDISVERSION_H
