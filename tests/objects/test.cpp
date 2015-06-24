#include "test.h"

#include <v8-cpp.h>

#include <gtest/gtest.h>

// OBJECTS
// construct class via new() from JS
// construct class via factory methods from JS (constructor overloads)
// move/ref objects to JS
// move/ref objects back to C++
// delete class from JS

// METHODS
// call regular class methods from JS
// call static class methods from JS
// call base class methods from JS (inheritance)
// call overridden class methods from JS (inheritance)
// call overloaded class methods from JS
// call template methods from JS??

// MEMBERS
// get/set class/struct members from JS

// FUNCTIONS
// call JS function from C++ (callback)
// call C++ functions from JS

TEST(Test, construct_class_via_new)
{
    v8::Isolate* isolate = v8::Isolate::New();

    auto test_object = v8cpp::run_script<TestClass>(isolate,
    R"(
        var module = require("./module");
        var test_object = new module.TestClass(1, 2);
        test_object;
    )");

    EXPECT_EQ(test_object.i_, 3);

    isolate->Dispose();
}

TEST(Test, construct_class_via_factory)
{
    v8::Isolate* isolate = v8::Isolate::New();

    auto test_object = v8cpp::run_script<TestClass>(isolate,
    R"(
        var module = require("./module");
        var test_object = module.new_TestClass(1, 2);
        test_object;
    )");

    EXPECT_EQ(test_object.i_, 3);

    isolate->Dispose();
}
