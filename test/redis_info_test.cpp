//
// Created by morgan on 12.02.2019.
//

#include "gtest/gtest.h"
#include "../RESP/Encoder.h"
#include "../RESP/Decoder.h"
#include "../redis_api/Info.h"

TEST(redis_info_test, positive_cases)
{
    auto e = Encoder();
    auto data = "# Server\r\nversion:1.0.0\r\nname:redis\r\n# Options\r\ntimeout:1000\r\n";
    e.write_bulk_string(data, strlen(data));
    auto d = Decoder(e.to_string().c_str());
    auto info = Info(d);

    auto sect = info.get("Server");
    EXPECT_EQ("1.0.0", sect.get("version"));
    EXPECT_EQ("redis", sect.get("name"));

    sect = info.get("Options");
    EXPECT_EQ("1000", sect.get("timeout"));
    EXPECT_EQ("", sect.get("version"));
}
