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
    e.write(123);
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

