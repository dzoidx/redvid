//
// Created by morgan on 03.02.2019.
//

#include "Encoder.h"
#include <string>
#include <cstring>

Encoder::Encoder()
{
}

void Encoder::write_int(int num)
{
    auto std_str = std::to_string(num);
    auto str = std_str.c_str();
    auto len = std_str.size();
    data_ << ':';
    data_ << std_str;
    data_ << "\r\n";
}

void Encoder::write_error(const char* error_msg)
{
    auto len = strlen(error_msg);
    data_ << "-";
    data_ << error_msg << "\r\n";
}

void Encoder::write_bulk_string(const char* str, int len)
{
    if(str == nullptr)
    {
        data_ << "$-1\r\n";
        return;
    }
    data_ << "$" << len << "\r\n";
    for(auto i = 0; i < len; ++i)
        data_ << str[i];
    data_ << "\r\n";
}

void Encoder::write_simple_string(const char* str)
{
    data_ << '+' << str << "\r\n";
}

void Encoder::write_array(int size)
{
    data_ << "*" << size << "\r\n";
}

std::string Encoder::to_string()
{
    return data_.str();
}
