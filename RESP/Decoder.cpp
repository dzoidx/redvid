//
// Created by morgan on 04.02.2019.
//

#include "Decoder.h"
#include <map>
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

Decoder::Decoder(const char* data, size_t len)
:data_(data, len), pos_(), error_()
{}

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

bool Decoder::read_endl()
{
    if(data_.size() == pos_)
    {
        error_.type = DecoderErrorType::FormatError;
        error_.position = pos_;
        return false;
    }
    ++pos_;
    if(data_[pos_] != '\n')
    {
        error_.type = DecoderErrorType::FormatError;
        error_.position = pos_;
        return false;
    }
    ++pos_;
    return true;
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
    if(data_.size() < pos_)
    {
        error_.position = pos_;
        error_.type = DecoderErrorType::FormatError;
        return 0;
    }
    long long result = 0;
    int size = 0;
    while (data_.size() > pos_ && data_[pos_] != '\r')
    {
        result *= 10;
        if(data_[pos_] < '0' || data_[pos_] > '9')
        {
            error_.position = pos_;
            error_.type = DecoderErrorType::FormatError;
            return 0;
        }
        result += data_[pos_++] & 0x0F;
        ++size;
    }
    if(size < 1)
    {
        error_.position = pos_;
        error_.type = DecoderErrorType::FormatError;
        return 0;
    }
    if(!read_endl())
        return 0;
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
    while (data_.size() > pos_ && data_[pos_] != '\r')
    {
        string_data << data_[pos_++];
    }
    if(!read_endl())
        return "";
    return string_data.str();
}

std::string Decoder::read_error()
{
    if(!error_.can_read())
        return "";
    if(data_[pos_] != '-')
    {
        error_.position = pos_;
        error_.type = DecoderErrorType::WrongType;
        return "";
    }

    ++pos_;
    std::stringstream string_data;
    while (data_.size() > pos_ && data_[pos_] != '\r')
    {
        string_data << data_[pos_++];
    }
    if(!read_endl())
        return "";
    return string_data.str();
}

std::shared_ptr<char> Decoder::read_bulk_string(int& size)
{
    if(!error_.can_read())
        return std::shared_ptr<char>(new char[0]);
    if(data_[pos_] != '$')
    {
        error_.position = pos_;
        error_.type = DecoderErrorType::WrongType;
        return std::shared_ptr<char>(new char[0]);
    }

    ++pos_;
    if(data_.size() < pos_)
    {
        error_.position = pos_;
        error_.type = DecoderErrorType::FormatError;
        return std::shared_ptr<char>(new char[0]);
    }
    bool neg = data_[pos_] == '-';
    if(neg)
        ++pos_;
    int str_len = 0;
    int size_len = 0;
    while (data_.size() > pos_ && data_[pos_] != '\r')
    {
        str_len *= 10;
        if(data_[pos_] < '0' || data_[pos_] > '9')
        {
            error_.position = pos_;
            error_.type = DecoderErrorType::FormatError;
            return std::shared_ptr<char>(new char[0]);
        }
        str_len += data_[pos_++] & 0x0F;
        ++size_len;
    }
    if(size_len == 0)
    {
        error_.position = pos_;
        error_.type = DecoderErrorType::FormatError;
        return std::shared_ptr<char>(new char[0]);
    }
    if(!read_endl())
        return std::shared_ptr<char>(new char[0]);
    if(neg)
    {
        if(str_len != 1)
        {
            error_.position = pos_;
            error_.type = DecoderErrorType::FormatError;
            return std::shared_ptr<char>(new char[0]);
        }
        return std::shared_ptr<char>();
    }
    if(data_.size() - pos_ < str_len)
    {
        error_.position = data_.size();
        error_.type = DecoderErrorType::FormatError;
        return std::shared_ptr<char>(new char[0]);
    }

    size = str_len;
    auto result = std::shared_ptr<char>(new char[str_len]);
    memcpy(result.get(), data_.c_str() + pos_, str_len);
    pos_ += str_len;

    if(!read_endl())
        return std::shared_ptr<char>(new char[0]);
    return result;
}

int Decoder::read_array_size()
{
    if(!error_.can_read())
        return 0;
    if(data_[pos_] != '*') {
        error_.position = pos_;
        error_.type = DecoderErrorType::WrongType;
        return 0;
    }

    ++pos_;
    if(data_.size() < pos_)
    {
        error_.position = pos_;
        error_.type = DecoderErrorType::FormatError;
        return 0;
    }
    int result = 0;
    int size = 0;
    while (data_.size() > pos_ && data_[pos_] != '\r')
    {
        result *= 10;
        if(data_[pos_] < '0' || data_[pos_] > '9')
        {
            error_.position = pos_;
            error_.type = DecoderErrorType::FormatError;
            return 0;
        }
        result += data_[pos_++] & 0x0F;
        ++size;
    }
    if(size < 1)
    {
        error_.position = pos_;
        error_.type = DecoderErrorType::FormatError;
        return 0;
    }
    if(!read_endl())
        return 0;
    return result;
}