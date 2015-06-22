#pragma once

#include <gtest/gtest.h>

class TestClass
{
public:
    TestClass(int a, int b)
    {
        EXPECT_EQ(a, 1);
        EXPECT_EQ(b, 2);
        i_ = a + b;
    }

    int i_;
};

