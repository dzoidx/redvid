//
// Created by morgan on 04.02.2019.
//

#include "Decoder.h"
#include <map>
#include <utility>

std::map<char, DataType> TypeMap =
{
        std::make_pair('+', DataType::SimpleString),
        std::make_pair('-', DataType::Error),
        std::make_pair(':', DataType::Integer),
        std::make_pair('$', DataType::BulkString),
        std::make_pair('*', DataType::Array)
};


Decoder::Decoder(const char* data)
:data_(data), pos_()
{
}

DataType Decoder::peek_next()
{
    if(pos_ >= data_.size())
        return DataType::None;

    auto sym = data_[pos_];
    auto found = TypeMap.find(sym);
    if(found == TypeMap.end())
        return DataType::None;

    return found->second;
}

long long Decoder::read_int()
{
    if(data_[pos_] != ':')
        return 0;

    ++pos_;
    long long result = 0;
    while (data_[pos_] != '\r' && data_.size() > pos_)
    {
        result *= 10;
        result += data_[pos_++] & 0x0F;
    }
    ++pos_;
    return result;
}