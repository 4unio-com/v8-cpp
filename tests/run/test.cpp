#include "test.h"

#include <v8-cpp.h>

#include <gtest/gtest.h>

TEST(Test, run_test_script)
{
    v8::Isolate* isolate = v8::Isolate::New();

    auto test_object = v8cpp::run_script_file<TestClass>(isolate, "./scripts/test.js");

    EXPECT_EQ(test_object.i(), 3);

    isolate->Dispose();
}

TEST(Test, run_test_script_2)
{
    v8::Isolate* isolate = v8::Isolate::New();

    auto test_object = v8cpp::run_script_file<TestClass>(isolate, "./scripts/test2.js");

    EXPECT_EQ(test_object.i(), 7);

    isolate->Dispose();
}
