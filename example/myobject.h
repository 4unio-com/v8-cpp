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

#include <string>
#include <iostream>

class MyObject
{
public:
    explicit MyObject(double value);
    explicit MyObject(std::string const& value);
    virtual ~MyObject() = default;

    double plus_one();

    double add(MyObject const& obj1, MyObject const& obj2)
    {
        return obj1.value_ + obj2.value_;
    }

    virtual double value() const
    {
        return value_;
    }

private:
    double value_;
};

class MyObject2
{
public:
    MyObject2(double value, int value2);
    virtual ~MyObject2() = default;

    double plus_two();
    int plus_two(int x);

    void base_method()
    {
        std::cout << "hello!\n";
        std::cout.flush();
    }

    static void static_method()
    {
        std::cout << "static hello!\n";
        std::cout.flush();
    }

private:
    double value_;
};

struct MyStruct
{
    bool bool_value;
    int int_value;
    std::string string_value = "hello";

    void output()
    {
        std::cout << string_value << std::endl;
        std::cout.flush();
    }
};
