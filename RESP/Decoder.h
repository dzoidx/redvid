//
// Created by morgan on 04.02.2019.
//

#ifndef REDVID_DECODER_H
#define REDVID_DECODER_H
#include <string>

enum class DataType
{
    None,
    SimpleString,
    Error,
    Integer,
    BulkString,
    Array
};

class Decoder
{
public:
    Decoder(const char* data);
    DataType peek_next();
    long long read_int();

private:
    std::string data_;
    int pos_;
};


#endif //REDVID_DECODER_H
