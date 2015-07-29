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
v8::Handle<v8::Value> call_v8(v8::Isolate* isolate, v8::Handle<v8::Function> func, Args... args)
{
    if (v8::Locker::IsActive() && !v8::Locker::IsLocked(isolate))
    {
        v8::Locker l(isolate);
        v8::Isolate::Scope isolate_scope(isolate);
        v8::HandleScope handle_scope(isolate);
        v8::Context::Scope context_scope(v8::Context::New(isolate));

        v8::EscapableHandleScope scope(isolate);

        int const arg_count = sizeof...(Args);

        // +1 for when arg_count == 0
        v8::Handle<v8::Value> v8_args[arg_count + 1] = {to_v8(isolate, args)...};

        v8::Local<v8::Value> result = func->Call(isolate->GetCurrentContext()->Global(), arg_count, v8_args);

        return scope.Escape(result);
    }
    else
    {
        v8::EscapableHandleScope scope(isolate);

        int const arg_count = sizeof...(Args);

        // +1 for when arg_count == 0
        v8::Handle<v8::Value> v8_args[arg_count + 1] = {to_v8(isolate, args)...};

        v8::Local<v8::Value> result = func->Call(isolate->GetCurrentContext()->Global(), arg_count, v8_args);

        return scope.Escape(result);
    }
}

}  // namespace v8cpp
