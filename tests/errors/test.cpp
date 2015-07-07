#include "test.h"

#include <v8-cpp.h>

#include <gtest/gtest.h>

TEST(Test, non_existent_script)
{
    v8::Isolate* isolate = v8::Isolate::New();

    try
    {
        v8cpp::run_script_file(isolate, "./scripts/noexist.js");
    }
    catch (std::exception const& e)
    {
        EXPECT_STREQ(e.what(), "run_script_file(): Failed to locate script file: \"./scripts/noexist.js\"");
    }

    isolate->Dispose();
}

TEST(Test, non_existent_module)
{
    v8::Isolate* isolate = v8::Isolate::New();

    try
    {
        v8cpp::run_script(isolate, "require('./noexist-module')");
    }
    catch (std::exception const& e)
    {
        EXPECT_STREQ(e.what(), "Uncaught Error: dlopen failed: ./noexist-module: cannot open shared object file: No such file or directory");
    }

    isolate->Dispose();
}

TEST(Test, failing_module)
{
    v8::Isolate* isolate = v8::Isolate::New();

    try
    {
        v8cpp::run_script(isolate, "require('./test-errors-fail-module')");
    }
    catch (std::exception const& e)
    {
        EXPECT_STREQ(e.what(), "Uncaught Error: dlsym failed: ./test-errors-fail-module.so: undefined symbol: init_module");
    }

    isolate->Dispose();
}

TEST(Test, failing_script)
{
    v8::Isolate* isolate = v8::Isolate::New();

    try
    {
        v8cpp::run_script(isolate, "#!/usr/bin/env sh");
    }
    catch (std::exception const& e)
    {
        EXPECT_STREQ(e.what(), "run_script(): Failed to compile script.");
    }

    isolate->Dispose();
}

TEST(Test, failing_script_file)
{
    v8::Isolate* isolate = v8::Isolate::New();

    try
    {
        v8cpp::run_script_file(isolate, "./scripts/fail.js");
    }
    catch (std::exception const& e)
    {
        EXPECT_STREQ(e.what(), "run_script(): Failed to compile script file: \"./scripts/fail.js\"");
    }

    isolate->Dispose();
}

TEST(Test, incorrect_function_args)
{
    v8::Isolate* isolate = v8::Isolate::New();

    try
    {
        v8cpp::run_script(isolate,
        R"(
            var module = require("./test-errors-module");
            var test_object = new module.TestClass(1);
            test_object.i();
        )");
    }
    catch (std::exception const& e)
    {
        EXPECT_STREQ(e.what(), "Uncaught Error: argument count does not match the corresponding C++ function definition");
    }

    isolate->Dispose();
}

TEST(Test, conversion_errors)
{
    v8::Isolate* isolate = v8::Isolate::New();

    try
    {
        v8cpp::run_script<std::string>(isolate, "1");
    }
    catch (std::exception const& e)
    {
        EXPECT_STREQ(e.what(), "expected string value");
    }
    try
    {
        v8cpp::run_script<bool>(isolate, "'hello'");
    }
    catch (std::exception const& e)
    {
        EXPECT_STREQ(e.what(), "expected bool value");
    }
    try
    {
        v8cpp::run_script<int>(isolate, "'hello'");
    }
    catch (std::exception const& e)
    {
        EXPECT_STREQ(e.what(), "expected integer value");
    }
    try
    {
        v8cpp::run_script<float>(isolate, "'hello'");
    }
    catch (std::exception const& e)
    {
        EXPECT_STREQ(e.what(), "expected float value");
    }
    try
    {
        v8cpp::run_script<std::vector<char>>(isolate, "1");
    }
    catch (std::exception const& e)
    {
        EXPECT_STREQ(e.what(), "expected array value");
    }
    try
    {
        v8cpp::run_script<TestClass>(isolate, "1");
    }
    catch (std::exception const& e)
    {
        EXPECT_STREQ(e.what(), "expected an object");
    }

    isolate->Dispose();
}

TEST(Test, throw_from_module)
{
    v8::Isolate* isolate = v8::Isolate::New();

    try
    {
        v8cpp::run_script(isolate,
        R"(
            var module = require("./test-errors-module");
            var test_object = new module.TestClass(1, 2);
            test_object.throw_ex();
        )");
    }
    catch (std::exception const& e)
    {
        EXPECT_STREQ(e.what(), "Uncaught Error: BOOM!");
    }

    isolate->Dispose();
}
