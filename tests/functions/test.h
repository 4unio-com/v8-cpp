#pragma once

#include <gtest/gtest.h>

#include <v8-cpp.h>

class TestCaller
{
public:
    explicit TestCaller(v8::Local<v8::Function> const& cb)
        : cb_(v8::Isolate::GetCurrent(), cb)
    {
    }

    std::string call_me() const
    {
        v8::Local<v8::Function> cb = v8cpp::to_local<v8::Function>(v8::Isolate::GetCurrent(), cb_);
        auto result = v8cpp::call_v8(v8::Isolate::GetCurrent(), cb, "hello");
        return v8cpp::from_v8<std::string>(v8::Isolate::GetCurrent(), result);
    }

private:
    v8::Persistent<v8::Function> cb_;
};

std::string simple_function()
{
    return "hello there";
}

std::vector<float> complex_function(int first, std::string const& second, float third, bool forth)
{
    EXPECT_EQ(first, 4);
    EXPECT_EQ(second, "3");
    EXPECT_FLOAT_EQ(third, 2.1);
    EXPECT_EQ(forth, false);

    std::vector<float> num_list;
    num_list.push_back(first);
    num_list.push_back(stoi(second));
    num_list.push_back(third);
    num_list.push_back(forth);
    return num_list;
}
