/*
 * Copyright (C) 2015 Canonical Ltd
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authored by: Marcus Tomlinson <marcus.tomlinson@canonical.com>
 */

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
    v8::Isolate* isolate = v8::Isolate::New();

    auto result = v8cpp::run_script<std::string>(isolate,
    R"(
        var module = require("./test-functions-module");
        module.simple_function();
    )");

    EXPECT_EQ(result, "hello there");

    auto result2 = v8cpp::run_script<std::vector<float>>(isolate,
    R"(
        var module = require("./test-functions-module");
        module.complex_function(4, "3", 2.1, false);
    )");

    ASSERT_EQ(result2.size(), 4);
    EXPECT_FLOAT_EQ(result2[0], 4);
    EXPECT_FLOAT_EQ(result2[1], 3);
    EXPECT_FLOAT_EQ(result2[2], 2.1);
    EXPECT_FLOAT_EQ(result2[3], 0);

    isolate->Dispose();
}
