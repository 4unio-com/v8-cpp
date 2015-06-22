#pragma once

#include <gtest/gtest.h>

class TestClass
{
public:
    TestClass(int i)
    {
        EXPECT_EQ(i, 73645);
        i_ = i;
    }

    int i_;
};

