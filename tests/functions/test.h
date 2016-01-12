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

class Shared
{
public:
    std::string get_value() const
    {
      return std::string("hello");
    }
};

class TestCaller
{
public:
    enum class TestEnum
    {
        ZERO,
        ONE,
        TWO,
        THREE
    };

    explicit TestCaller(v8::Local<v8::Function> const& cb)
        : cb_(v8::Isolate::GetCurrent(), cb)
    {
    }

    TestEnum return_enum_function() const
    {
      return TestEnum::ONE;
    }

    std::string call_me() const
    {
        v8::Local<v8::Function> cb = v8cpp::to_local<v8::Function>(v8::Isolate::GetCurrent(), cb_);
        auto result = v8cpp::call_v8(v8::Isolate::GetCurrent(), cb, "hello");
        return v8cpp::from_v8<std::string>(v8::Isolate::GetCurrent(), result);
    }

    std::shared_ptr<const Shared> get_shared()
    {
        auto s = std::shared_ptr<const Shared>(new Shared());
        return s;
    }

    v8::Local<v8::Value>
      call_me_with_shared(v8::Local<v8::Object> receiver,
                          v8::Local<v8::Function> to_be_called_with_shared)
    {
        return v8cpp::call_v8_with_receiver(v8::Isolate::GetCurrent(), receiver, to_be_called_with_shared);
    }

private:
    v8::Persistent<v8::Function> cb_;
};

std::string simple_function()
{
    return "hello there";
}

std::vector<float> complex_function(int first, std::string const& second, float third, bool forth)
{
    EXPECT_EQ(first, 4);
    EXPECT_EQ(second, "3");
    EXPECT_FLOAT_EQ(third, 2.1);
    EXPECT_EQ(forth, false);

    std::vector<float> num_list;
    num_list.push_back(first);
    num_list.push_back(stoi(second));
    num_list.push_back(third);
    num_list.push_back(forth);
    return num_list;
}

std::list<float> complex_list_function(int first, std::string const& second, float third, bool forth)
{
    EXPECT_EQ(first, 4);
    EXPECT_EQ(second, "3");
    EXPECT_FLOAT_EQ(third, 2.1);
    EXPECT_EQ(forth, false);

    std::list<float> nums;
    nums.push_back(first);
    nums.push_back(stoi(second));
    nums.push_back(third);
    nums.push_back(forth);
    return nums;
}

std::set<std::string> complex_set_function(int first, std::string const& second, bool third)
{
    EXPECT_EQ(first, 4);
    EXPECT_EQ(second, "3");
    EXPECT_EQ(third, false);

    std::set<std::string> nums;
    nums.insert(std::to_string(first));
    nums.insert(second);
    nums.insert(std::to_string(third));
    return nums;
}
