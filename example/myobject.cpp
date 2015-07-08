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

#include "myobject.h"

MyObject::MyObject(double value)
{
    value_ = value;
}

MyObject::MyObject(std::string const& value)
{
    value_ = 1;
}

double MyObject::plus_one()
{
    value_ += 1;
    return value_;
}

MyObject2::MyObject2(double value, int value2)
{
    value_ = value;
}

double MyObject2::plus_two()
{
    value_ += 2;
    return value_;
}

int MyObject2::plus_two(int x)
{
    value_ += 2 + x;
    return value_;
}
