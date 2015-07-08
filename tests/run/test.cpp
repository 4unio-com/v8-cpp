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
