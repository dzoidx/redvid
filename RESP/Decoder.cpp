//
// Created by morgan on 04.02.2019.
//

#include "Decoder.h"
#include <map>
#include <utility>
#include <sstream>

std::map<char, DataType> TypeMap =
{
        std::make_pair('+', DataType::SimpleString),
        std::make_pair('-', DataType::Error),
        std::make_pair(':', DataType::Integer),
        std::make_pair('$', DataType::BulkString),
        std::make_pair('*', DataType::Array)
};


Decoder::Decoder(const char* data)
:data_(data), pos_(), error_()
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
    if(!error_.can_read())
        return 0;
    if(data_[pos_] != ':') {
        error_.position = pos_;
        error_.type = DecoderErrorType::WrongType;
        return 0;
    }

    ++pos_;
    long long result = 0;
    while (data_[pos_] != '\r' && data_.size() > pos_)
    {
        result *= 10;
        if(data_[pos_] < '0' || data_[pos_] > '9')
        {
            error_.position = pos_;
            error_.type = DecoderErrorType::FormatError;
            return 0;
        }
        result += data_[pos_++] & 0x0F;
    }
    if(data_.size() == pos_)
    {
        error_.type = DecoderErrorType::FormatError;
        error_.position = pos_;
        return 0;
    }
    ++pos_;
    if(data_[pos_] != '\n')
    {
        error_.type = DecoderErrorType::FormatError;
        error_.position = pos_;
        return 0;
    }
    ++pos_;
    return result;
}

std::string Decoder::read_simple_string()
{
    if(!error_.can_read())
        return "";
    if(data_[pos_] != '+')
    {
        error_.position = pos_;
        error_.type = DecoderErrorType::WrongType;
        return "";
    }

    ++pos_;
    std::stringstream string_data;
    while (data_[pos_] != '\r' && data_.size() > pos_)
    {
        string_data << data_[pos_++];
    }
    if(data_.size() == pos_)
    {
        error_.type = DecoderErrorType::FormatError;
        error_.position = pos_;
        return "";
    }
    ++pos_;
    if(data_[pos_] != '\n')
    {
        error_.type = DecoderErrorType::FormatError;
        error_.position = pos_;
        return "";
    }
    ++pos_;
    return string_data.str();
}