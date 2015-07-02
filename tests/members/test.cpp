#include "test.h"

#include <v8-cpp.h>

#include <gtest/gtest.h>

// MEMBERS
// get/set class/struct members from JS

TEST(Test, get_set_members_from_js)
{
    v8::Isolate* isolate = v8::Isolate::New();

    auto test_struct = v8cpp::run_script<TestStruct>(isolate,
    R"(
        var module = require("./test-members-module");
        var test_struct = new module.TestStruct();
        test_struct;
    )");

    EXPECT_EQ(test_struct.bool_value, true);
    EXPECT_EQ(test_struct.int_value, 9);
    EXPECT_FLOAT_EQ(test_struct.float_value, 0.1);
    EXPECT_EQ(test_struct.string_value, "hello");

    test_struct = v8cpp::run_script<TestStruct>(isolate,
    R"(
        var module = require("./test-members-module");
        var test_struct = new module.TestStruct();
        if (test_struct.bool_value)
        {
            test_struct.bool_value = false;
            test_struct.int_value = -1;
            test_struct.float_value = test_struct.int_value + 0.12;
            test_struct.string_value = test_struct.string_value + " there";
        }
        test_struct;
    )");

    EXPECT_EQ(test_struct.bool_value, false);
    EXPECT_EQ(test_struct.int_value, -1);
    EXPECT_FLOAT_EQ(test_struct.float_value, -0.88);
    EXPECT_EQ(test_struct.string_value, "hello there");

    isolate->Dispose();
}
