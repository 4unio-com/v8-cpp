#include "test.h"

#include <v8-cpp.h>

#include <gtest/gtest.h>

// MEMBERS
// get/set class/struct members from JS

TEST(Test, basic)
{
    v8::Isolate* isolate = v8::Isolate::New();

    auto test_object = v8cpp::run_script<TestClass>(isolate,
    R"(
        var module = require("./test-members-module");
        var test_object = new module.TestClass();
        test_object;
    )");

    isolate->Dispose();
}
