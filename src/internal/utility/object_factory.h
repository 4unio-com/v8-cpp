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

// A factory that constructs and destructs a C++ object (callable from V8)
template <typename T>
struct ObjectFactory
{
    static size_t const object_size = sizeof(T);

    template <typename... Args>
    static T* new_object(v8::Isolate* isolate, Args... args)
    {
        T* object = new T(std::forward<Args>(args)...);
        isolate->AdjustAmountOfExternalAllocatedMemory(static_cast<int64_t>(object_size));
        return object;
    }

    static void delete_object(v8::Isolate* isolate, T* object)
    {
        delete object;
        isolate->AdjustAmountOfExternalAllocatedMemory(-static_cast<int64_t>(object_size));
    }
};

}  // namespace internal
}  // namespace v8cpp
