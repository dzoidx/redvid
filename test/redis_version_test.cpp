//
// Created by morgan on 08.02.2019.
//

#include "gtest/gtest.h"
#include "../redis_api/RedisVersion.h"

TEST(redis_version_test, positive_cases)
{
    auto v = RedisVersion("1.2.3");
    auto expected = 0x01002003;

    EXPECT_EQ(expected, (unsigned int)v);

    auto f_v = RedisVersion(1);
    auto s_v = RedisVersion(1, 2, 3);

    EXPECT_NE(s_v, f_v);
    EXPECT_GT(s_v, f_v);
    EXPECT_GE(s_v, f_v);
    EXPECT_LT(f_v, s_v);
    EXPECT_LE(f_v, s_v);
    EXPECT_EQ(s_v, v);

    s_v = RedisVersion(1, 0, 3);
    EXPECT_GT(s_v, f_v);
    EXPECT_LT(f_v, s_v);

    f_v = RedisVersion("2.0");
    s_v = RedisVersion("10.0");
    EXPECT_LT(f_v, s_v);

    f_v = RedisVersion(2,28,0);
    s_v = RedisVersion(2,3,0);
    EXPECT_GT(f_v, s_v);
}

TEST(redis_version_test, negative_cases)
{
    auto v = RedisVersion("bad format.");

    EXPECT_EQ(0, (int)v);

    v = RedisVersion("..");
    EXPECT_EQ(0, (int)v);

    v = RedisVersion("a.b.c");
    EXPECT_EQ(0, (int)v);
}
