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

#include <memory>

class TestClass
{
public:
    TestClass(int a, int b)
    {
        i_ = a + b;
    }

    int i() const
    {
        return i_;
    }

    int expect_sptr(std::shared_ptr<TestClass> x) const
    {
        return 1;
    }

    int expect_obj(TestClass x) const
    {
        return 1;
    }

    void throw_ex()
    {
        throw std::runtime_error("BOOM!");
    }

private:
    int i_;
};
