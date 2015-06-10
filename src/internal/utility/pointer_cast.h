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

namespace v8cpp
{
namespace internal
{

template <typename T>
using IsPointerCastAllowed = std::integral_constant<bool, sizeof(T) <= sizeof(void*)>;

template <typename T>
union PointerCast
{
public:
    static_assert(IsPointerCastAllowed<T>::value, "data size can not be greater than the pointer size");

    explicit PointerCast(void* ptr)
        : ptr_(ptr)
    {
    }
    explicit PointerCast(T value)
        : value_(value)
    {
    }

    operator void*() const
    {
        return ptr_;
    }
    operator T() const
    {
        return value_;
    }

private:
    void* ptr_;
    T value_;
};

}  // namespace internal
}  // namespace v8cpp
