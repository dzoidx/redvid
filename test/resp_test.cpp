//
// Created by morgan on 04.02.2019.
//

#include <variant>
#include "gtest/gtest.h"
#include "../RESP/Encoder.h"
#include "../RESP/Decoder.h"

TEST(RESP_test, encode_int)
{
    auto expected = std::string(":123\r\n");
    auto e = Encoder();
    e.write_int(123);
    auto result = e.to_string();

    EXPECT_EQ(expected, result);
}

TEST(RESP_test, encode_error)
{
    auto expected = std::string("-Some error\r\n");
    auto e = Encoder();
    e.write_error("Some error");
    auto result = e.to_string();

    EXPECT_EQ(expected, result);
}

TEST(RESP_test, encode_simple_string)
{
    auto expected = std::string("+Some string\r\n");
    auto e = Encoder();
    e.write_simple_string("Some string");
    auto result = e.to_string();

    EXPECT_EQ(expected, result);
}

TEST(RESP_test, encode_bulk_string)
{
    auto expected = std::string("$12\r\nSome\r\nstring\r\n");
    auto e = Encoder();
    e.write_bulk_string("Some\r\nstring", 12);
    auto result = e.to_string();

    EXPECT_EQ(expected, result);
}

TEST(RESP_test, encode_empty_bulk_string)
{
    auto expected = std::string("$0\r\n\r\n");
    auto e = Encoder();
    e.write_bulk_string("", 0);
    auto result = e.to_string();

    EXPECT_EQ(expected, result);
}

TEST(RESP_test, encode_null_bulk_string)
{
    auto expected = std::string("$-1\r\n");
    auto e = Encoder();
    e.write_bulk_string(nullptr, 0);
    auto result = e.to_string();

    EXPECT_EQ(expected, result);
}

TEST(RESP_test, encode_array)
{
    auto expected = std::string("*3\r\n");
    auto e = Encoder();
    e.write_array(3);
    auto result = e.to_string();

    EXPECT_EQ(expected, result);
}

TEST(RESP_test, encode_null_array)
{
    auto expected = std::string("*-1\r\n");
    auto e = Encoder();
    e.write_null_array();
    auto result = e.to_string();

    EXPECT_EQ(expected, result);
}

TEST(RESP_test, encode_bulk_string_array)
{
    auto expected = std::string("*3\r\n$12\r\nSome\r\nstring\r\n$12\r\nSome\r\nstring\r\n$12\r\nSome\r\nstring\r\n");
    auto e = Encoder();
    e.write_array(3);
    e.write_bulk_string("Some\r\nstring", 12);
    e.write_bulk_string("Some\r\nstring", 12);
    e.write_bulk_string("Some\r\nstring", 12);
    auto result = e.to_string();

    EXPECT_EQ(expected, result);
}

TEST(RESP_test, decode_int)
{
    auto d = Decoder(":123\r\n");
    auto type = d.peek_next();

    EXPECT_EQ(DataType::Integer, type);

    auto expected = 123;
    auto num = d.read_int();

    EXPECT_EQ(expected, num);
}

TEST(RESP_test, decode_int_error)
{
    auto d = Decoder("+string\r\n");
    auto result = d.read_int();
    auto error = d.get_error();
    EXPECT_TRUE(error.has_error());
    EXPECT_EQ(0, error.position);
    EXPECT_TRUE(error.can_read());

    d = Decoder(":string\r\n");
    result = d.read_int();
    error = d.get_error();
    EXPECT_TRUE(error.has_error());
    EXPECT_EQ(1, error.position);
    EXPECT_FALSE(error.can_read());

    result = d.read_int();
    error = d.get_error();
    EXPECT_TRUE(error.has_error());
    EXPECT_EQ(1, error.position);
    EXPECT_FALSE(error.can_read());

    d = Decoder(":123\r");
    result = d.read_int();
    error = d.get_error();
    EXPECT_TRUE(error.has_error());
    EXPECT_EQ(5, error.position);
    EXPECT_FALSE(error.can_read());

    d = Decoder(":123");
    result = d.read_int();
    error = d.get_error();
    EXPECT_TRUE(error.has_error());
    EXPECT_EQ(4, error.position);
    EXPECT_FALSE(error.can_read());
}

TEST(RESP_test, decode_simple_string)
{
    auto d = Decoder("+123 some string\r\n");
    auto type = d.peek_next();

    EXPECT_EQ(DataType::SimpleString, type);

    auto expected = "123 some string";
    auto str = d.read_simple_string();

    EXPECT_EQ(expected, str);
}

TEST(RESP_test, decode_simple_string_error)
{
    auto d = Decoder(":123 some string\r\n");
    auto result = d.read_simple_string();
    auto error = d.get_error();
    EXPECT_TRUE(error.has_error());
    EXPECT_EQ(0, error.position);
    EXPECT_TRUE(error.can_read());

    d = Decoder("+123 some string\r");
    result = d.read_simple_string();
    error = d.get_error();
    EXPECT_TRUE(error.has_error());
    EXPECT_EQ(17, error.position);
    EXPECT_FALSE(error.can_read());

    d = Decoder("+123 some string");
    result = d.read_simple_string();
    error = d.get_error();
    EXPECT_TRUE(error.has_error());
    EXPECT_EQ(16, error.position);
    EXPECT_FALSE(error.can_read());
}

TEST(RESP_test, decode_error)
{
    auto d = Decoder("-Some error\r\n");
    auto type = d.peek_next();

    EXPECT_EQ(DataType::Error, type);

    auto expected = "Some error";
    auto str = d.read_error();

    EXPECT_EQ(expected, str);
}

TEST(RESP_test, decode_error_parse_error)
{
    auto d = Decoder(":Some error\r\n");
    auto result = d.read_error();
    auto error = d.get_error();
    EXPECT_TRUE(error.has_error());
    EXPECT_EQ(0, error.position);
    EXPECT_TRUE(error.can_read());

    d = Decoder("-Some error\r");
    result = d.read_error();
    error = d.get_error();
    EXPECT_TRUE(error.has_error());
    EXPECT_EQ(12, error.position);
    EXPECT_FALSE(error.can_read());

    d = Decoder("-Some error");
    result = d.read_error();
    error = d.get_error();
    EXPECT_TRUE(error.has_error());
    EXPECT_EQ(11, error.position);
    EXPECT_FALSE(error.can_read());
}

TEST(RESP_test, decode_bulk_string)
{
    auto d = Decoder("$12\r\nBulk\r\nstring\r\n");
    auto type = d.peek_next();

    EXPECT_EQ(DataType::BulkString, type);

    auto expected = "Bulk\r\nstring";
    int size;
    auto ptr = d.read_bulk_string(size);
    auto str = std::string(ptr.get(), size);

    EXPECT_EQ(expected, str);

    d = Decoder("$-1\r\n");
    ptr = d.read_bulk_string(size);

    EXPECT_EQ(nullptr, ptr.get());

    d = Decoder("$0\r\n\r\n");
    ptr = d.read_bulk_string(size);

    EXPECT_EQ(0, size);
    EXPECT_EQ(0, memcmp("", ptr.get(), 0));
}

TEST(RESP_test, decode_bulk_string_error)
{
    auto d = Decoder("+String\r\n");
    int size;
    auto result = d.read_bulk_string(size);
    auto error = d.get_error();

    EXPECT_TRUE(error.has_error());
    EXPECT_EQ(0, error.position);
    EXPECT_TRUE(error.can_read());

    d = Decoder("$String\r\n");
    result = d.read_bulk_string(size);
    error = d.get_error();

    EXPECT_TRUE(error.has_error());
    EXPECT_EQ(1, error.position);
    EXPECT_FALSE(error.can_read());

    d = Decoder("$\r\n");
    result = d.read_bulk_string(size);
    error = d.get_error();

    EXPECT_TRUE(error.has_error());
    EXPECT_EQ(1, error.position);
    EXPECT_FALSE(error.can_read());

    d = Decoder("$12\r\n");
    result = d.read_bulk_string(size);
    error = d.get_error();

    EXPECT_TRUE(error.has_error());
    EXPECT_EQ(5, error.position);
    EXPECT_FALSE(error.can_read());

    d = Decoder("$12\r\nBulk\r\n");
    result = d.read_bulk_string(size);
    error = d.get_error();

    EXPECT_TRUE(error.has_error());
    EXPECT_EQ(11, error.position);
    EXPECT_FALSE(error.can_read());

    d = Decoder("$12\r\nBulk\r\nstring");
    result = d.read_bulk_string(size);
    error = d.get_error();

    EXPECT_TRUE(error.has_error());
    EXPECT_EQ(17, error.position);
    EXPECT_FALSE(error.can_read());
}