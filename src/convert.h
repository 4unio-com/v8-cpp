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

#include <internal/convert.h>

#include <v8.h>

#include <climits>
#include <vector>
#include <map>

namespace v8cpp
{

template <typename T>
typename internal::Convert<T>::FromType from_v8(v8::Isolate* isolate, v8::Handle<v8::Value> value)
{
    internal::Locker l(isolate);
    return internal::Convert<T>::from_v8(isolate, value);
}

template <typename T, typename U>
typename internal::Convert<T>::FromType from_v8(v8::Isolate* isolate,
                                                v8::Handle<v8::Value> value,
                                                U const& default_value)
{
    internal::Locker l(isolate);
    return internal::Convert<T>::is_valid(isolate, value) ? internal::Convert<T>::from_v8(isolate, value) :
                                                            default_value;
}

inline v8::Handle<v8::String> to_v8(v8::Isolate* isolate, char const* str, int len = -1)
{
    internal::Locker l(isolate);
    return v8::String::NewFromUtf8(isolate, str, v8::String::kNormalString, len);
}

template <typename T>
typename internal::Convert<T>::ToType to_v8(v8::Isolate* isolate, T const& value)
{
    internal::Locker l(isolate);
    return internal::Convert<T>::to_v8(isolate, value);
}

inline v8::Handle<v8::Value> to_v8(v8::Isolate* isolate, std::nullptr_t)
{
    internal::Locker l(isolate);
    return v8::Null(isolate);
}

template <typename I>
v8::Handle<v8::Array> to_v8(v8::Isolate* isolate, I begin, I end)
{
    internal::Locker l(isolate);
    v8::EscapableHandleScope scope(isolate);

    v8::Local<v8::Array> result = v8::Array::New(isolate);
    for (uint32_t idx = 0; begin != end; ++begin, ++idx)
    {
        result->Set(idx, to_v8(isolate, *begin));
    }
    return scope.Escape(result);
}

template <typename T>
v8::Local<T> to_local(v8::Isolate* isolate, v8::PersistentBase<T> const& handle)
{
    internal::Locker l(isolate);
    if (handle.IsWeak())
    {
        return v8::Local<T>::New(isolate, handle);
    }
    else
    {
        return *reinterpret_cast<v8::Local<T>*>(const_cast<v8::PersistentBase<T>*>(&handle));
    }
}

}  // namespace v8cpp
