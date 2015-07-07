#pragma once

#include <gtest/gtest.h>

class TestClass
{
public:
    TestClass(int a, int b)
    {
        i_ = a + b;
    }

    int i() const
    {
        return i_;
    }

private:
    int i_;
};
