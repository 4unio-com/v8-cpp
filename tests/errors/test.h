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

    void throw_ex()
    {
        throw std::runtime_error("BOOM!");
    }

private:
    int i_;
};
