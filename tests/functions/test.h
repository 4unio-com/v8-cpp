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

    std::string call_me()
    {
        v8::Local<v8::Function> cb = v8cpp::to_local<v8::Function>(v8::Isolate::GetCurrent(), cb_);
        auto result = v8cpp::call_v8(v8::Isolate::GetCurrent(), cb, "hello");
        return v8cpp::from_v8<std::string>(v8::Isolate::GetCurrent(), result);
    }

private:
    v8::Persistent<v8::Function> cb_;
};
