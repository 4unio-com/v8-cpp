#include "test.h"

#include <v8-cpp.h>

#include <gtest/gtest.h>

// METHODS
// call regular class methods from JS
// call static class methods from JS
// call base class methods from JS (inheritance)
// call overridden class methods from JS (inheritance)
// call overloaded class methods from JS
// call template methods from JS??

TEST(Test, basic)
{
    v8::Isolate* isolate = v8::Isolate::New();

    auto test_object = v8cpp::run_script<TestClass>(isolate,
    R"(
        var module = require("./test-methods-module");
        var test_object = new module.TestClass();
        test_object;
    )");

    isolate->Dispose();
}
