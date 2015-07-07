#include "test.h"

#include <v8-cpp.h>

#include <gtest/gtest.h>

// METHODS
// call regular class methods from JS
// call static class methods from JS
// call base class methods from JS (inheritance)
// call overridden class methods from JS (inheritance)
// call overloaded class methods from JS

TEST(Test, call_regular_method_from_js)
{
    v8::Isolate* isolate = v8::Isolate::New();

    auto result = v8cpp::run_script<int>(isolate,
    R"(
        var module = require("./test-methods-module");
        var test_object = new module.TestClass();
        test_object.regular_method();
    )");

    EXPECT_EQ(result, 2);

    isolate->Dispose();
}

TEST(Test, call_static_method_from_js)
{
    v8::Isolate* isolate = v8::Isolate::New();

    auto result = v8cpp::run_script<int>(isolate,
    R"(
        var module = require("./test-methods-module");
        module.TestClass.static_method();
    )");

    EXPECT_EQ(result, 3);

    isolate->Dispose();
}

TEST(Test, call_base_method_from_js)
{
    v8::Isolate* isolate = v8::Isolate::New();

    auto result = v8cpp::run_script<int>(isolate,
    R"(
        var module = require("./test-methods-module");
        var test_object = new module.TestClass();
        test_object.base_method();
    )");

    EXPECT_EQ(result, 1);

    result = v8cpp::run_script<int>(isolate,
    R"(
        var module = require("./test-methods-module");
        var test_object = new module.TestClass();
        test_object.base_base_method();
    )");

    EXPECT_EQ(result, 0);

    isolate->Dispose();
}

TEST(Test, call_override_method_from_js)
{
    v8::Isolate* isolate = v8::Isolate::New();

    auto result = v8cpp::run_script<int>(isolate,
    R"(
        var module = require("./test-methods-module");
        var test_object = new module.TestClass();
        test_object.virtual_method();
    )");

    EXPECT_EQ(result, 4);

    isolate->Dispose();
}

TEST(Test, call_overload_method_from_js)
{
    v8::Isolate* isolate = v8::Isolate::New();

    auto result = v8cpp::run_script<int>(isolate,
    R"(
        var module = require("./test-methods-module");
        var test_object = new module.TestClass();
        test_object.overload_method(5);
    )");

    EXPECT_EQ(result, 5);

    auto result2 = v8cpp::run_script<std::string>(isolate,
    R"(
        var module = require("./test-methods-module");
        var test_object = new module.TestClass();
        test_object.overload_method(1, 5);
    )");

    EXPECT_EQ(result2, "6");

    isolate->Dispose();
}
