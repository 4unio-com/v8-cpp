#include "test.h"

#include <v8-cpp.h>

#include <gtest/gtest.h>

// FUNCTIONS
// call JS function from C++ (callback)
// call C++ functions from JS

TEST(Test, call_to_js)
{
    v8::Isolate* isolate = v8::Isolate::New();

    auto callback_result = v8cpp::run_script<std::string>(isolate,
    R"(
        var module = require("./test-functions-module");

        var caller = new module.TestCaller(function(message)
        {
            return message + " world" // "hello world"
        });

        caller.call_me();
    )");

    EXPECT_EQ(callback_result, "hello world");

    isolate->Dispose();
}

TEST(Test, call_from_js)
{
}
