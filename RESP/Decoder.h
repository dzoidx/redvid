//
// Created by morgan on 04.02.2019.
//

#ifndef REDVID_DECODER_H
#define REDVID_DECODER_H
#include <string>
#include <memory>

enum class DataType
{
    None,
    SimpleString,
    Error,
    Integer,
    BulkString,
    Array
};

enum class DecoderErrorType
{
    None,
    WrongType,
    FormatError
};

struct DecoderError
{
    int position;
    DecoderErrorType type;

    bool has_error() const { return type != DecoderErrorType::None; }
    bool can_read() const { return !has_error() || type != DecoderErrorType::FormatError; }
};

class Decoder
{
public:
    Decoder(const char* data);
    Decoder(const char* data, size_t len);
    DataType peek_next();
    long long read_int();
    std::string read_simple_string();
    std::string read_error();
    std::shared_ptr<char> read_bulk_string(int& size);
    DecoderError get_error() const { return error_; }

private:
    bool read_endl();

private:
    std::string data_;
    int pos_;
    DecoderError error_;
};


#endif //REDVID_DECODER_H
