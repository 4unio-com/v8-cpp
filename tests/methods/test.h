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

#pragma once

#include <gtest/gtest.h>

#include <v8-cpp.h>

class BaseBaseTestClass
{
public:
    virtual ~BaseBaseTestClass() = default;

    int base_base_method()
    {
        return 0;
    }

    virtual int virtual_method() = 0;
};

class BaseTestClass : public BaseBaseTestClass
{
public:
    virtual ~BaseTestClass() = default;

    int base_method()
    {
        return 1;
    }
};

class TestClass : public BaseTestClass
{
public:
    virtual ~TestClass() = default;

    int regular_method()
    {
        return 2;
    }

    static int static_method()
    {
        return 3;
    }

    int virtual_method() override
    {
        return 4;
    }

    int overload_method(int a)
    {
        return a;
    }

    std::string overload_method(int a, int b)
    {
        return std::to_string(a + b);
    }
};

class TestClass_OL : public TestClass
{
public:
    using TestClass::TestClass;

    v8::Local<v8::Value> overload_method(v8::FunctionCallbackInfo<v8::Value> const& args)
    {
        if (args.Length() == 1)
        {
            int arg = v8cpp::from_v8<int>(v8::Isolate::GetCurrent(), args[0]);
            return v8cpp::to_v8(v8::Isolate::GetCurrent(), TestClass::overload_method(arg));
        }
        else if (args.Length() == 2)
        {
            int arg = v8cpp::from_v8<int>(v8::Isolate::GetCurrent(), args[0]);
            int arg2 = v8cpp::from_v8<int>(v8::Isolate::GetCurrent(), args[1]);
            return v8cpp::to_v8(v8::Isolate::GetCurrent(), TestClass::overload_method(arg, arg2));
        }
        return v8cpp::to_v8(v8::Isolate::GetCurrent(), 0);
    }
};
