//
// https://redis.io/topics/protocol
//

#ifndef REDVID_ENCODER_H
#define REDVID_ENCODER_H
#include <string>
#include <vector>
#include <sstream>

class Encoder
{
public:
    Encoder();
    void write_int(int num);
    void write_bulk_string(const char* str, int len);
    void write_simple_string(const char* str);
    void write_error(const char* error_msg);
    void write_array(int size);
    void write_null_array() { write_array(-1);}
    std::string to_string();

private:
    void ensure_buffer(int size);

private:
    std::stringstream data_;
};


#endif //REDVID_ENCODER_H
