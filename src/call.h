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

#include <convert.h>

#include <v8.h>

namespace v8cpp
{

// Call a V8 function from C++
template <typename... Args>
v8::Local<v8::Value> call_v8_with_receiver(v8::Isolate* isolate, v8::Local<v8::Object> receiver, v8::Local<v8::Function> func, Args... args)
{
    v8::EscapableHandleScope scope(isolate);

    int const arg_count = sizeof...(Args);

    // +1 for when arg_count == 0
    v8::Local<v8::Value> v8_args[arg_count + 1] = {to_v8(isolate, args)...};

    v8::Local<v8::Value> result = func->Call(receiver, arg_count, v8_args);

    return scope.Escape(result);
}

// Call a V8 function from C++
template <typename... Args>
v8::Local<v8::Value> call_v8(v8::Isolate* isolate, v8::Local<v8::Function> func, Args... args)
{
  return call_v8_with_receiver(isolate, isolate->GetCurrentContext()->Global(), func, args...);
}

}  // namespace v8cpp
