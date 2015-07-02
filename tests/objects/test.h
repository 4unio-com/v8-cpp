#pragma once

#include <gtest/gtest.h>

class EmbeddedTestClass
{
public:
    EmbeddedTestClass(int a, int b)
    {
        EXPECT_EQ(a, 1);
        EXPECT_EQ(b, 2);
        i_ = a - b;
    }

    int i() const
    {
        return i_;
    }

private:
    int i_;
};

class TestClass
{
public:
    TestClass(int a, int b)
        : embedded_class_(a, b)
    {
        EXPECT_EQ(a, 1);
        EXPECT_EQ(b, 2);
        i_ = a + b;
    }

    int i() const
    {
        return i_;
    }

    EmbeddedTestClass* embedded_class_ptr()
    {
        return &embedded_class_;
    }

    EmbeddedTestClass& embedded_class_ref()
    {
        return embedded_class_;
    }

    EmbeddedTestClass embedded_class_copy()
    {
        return embedded_class_;
    }

    void replace_i(EmbeddedTestClass const& other)
    {
        i_ = other.i();
    }

    int add_i(EmbeddedTestClass const& other, EmbeddedTestClass const& other2)
    {
        return other.i() + other2.i();
    }

private:
    EmbeddedTestClass embedded_class_;
    int i_;
};

TestClass* new_TestClass(int a, int b)
{
    return new TestClass(a, b);
}
