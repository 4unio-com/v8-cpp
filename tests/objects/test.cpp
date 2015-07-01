#include "test.h"

#include <v8-cpp.h>

#include <gtest/gtest.h>

// OBJECTS
// construct class via new() from JS
// construct class via factory methods from JS (constructor overloads)
// move/ref objects to JS
// move/ref objects back to C++

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

    EXPECT_EQ(test_object.i(), 3);

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

    EXPECT_EQ(test_object.i(), 3);

    isolate->Dispose();
}

TEST(Test, object_to_js)
{
    v8::Isolate* isolate = v8::Isolate::New();

    auto test_object = v8cpp::run_script<EmbeddedTestClass*>(isolate,
    R"(
        var module = require("./module");
        var test_object = module.new_TestClass(1, 2);
        test_object.embedded_class_ptr();
    )");

    EXPECT_EQ(test_object->i(), -1);

    auto test_object2 = v8cpp::run_script<EmbeddedTestClass&>(isolate,
    R"(
        var module = require("./module");
        var test_object = module.new_TestClass(1, 2);
        test_object.embedded_class_ref();
    )");

    EXPECT_EQ(test_object2.i(), -1);

    auto test_object3 = v8cpp::run_script<EmbeddedTestClass>(isolate,
    R"(
        var module = require("./module");
        var test_object = module.new_TestClass(1, 2);
        test_object.embedded_class_copy();
    )");

    EXPECT_EQ(test_object3.i(), -1);

    isolate->Dispose();
}

TEST(Test, object_from_js)
{
    v8::Isolate* isolate = v8::Isolate::New();

    auto test_object = v8cpp::run_script<TestClass>(isolate,
    R"(
        var module = require("./module");
        var test_object = module.new_TestClass(1, 2);
        var test_object2 = module.new_TestClass(1, 2);

        test_object.replace_i(test_object2.embedded_class_copy());
        test_object;
    )");

    EXPECT_EQ(test_object.i(), -1);

    auto result = v8cpp::run_script<int>(isolate,
    R"(
        var module = require("./module");
        var test_object = module.new_TestClass(1, 2);
        var test_object2 = module.new_TestClass(1, 2);

        test_object.add_i(test_object.embedded_class_ptr(),
                          test_object2.embedded_class_ref());
    )");

    EXPECT_EQ(result, -2);

    isolate->Dispose();
}
