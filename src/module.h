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

#include <internal/function.h>

namespace v8cpp
{

template <typename T>
class Class;

// Module (abstraction of v8::ObjectTemplate)
class Module
{
public:
    explicit Module(v8::Isolate* isolate)
        : isolate_(isolate)
        , obj_(v8::ObjectTemplate::New(isolate))
    {
    }

    // Add a C++ class to the module
    template <typename T>
    Module& add_class(char const* name, Class<T>& cl)
    {
        obj_->Set(to_v8(isolate_, name), cl.class_.proto_template()->GetFunction());
        return *this;
    }

    // Add a C++ function to the module
    template <typename F>
    Module& add_function(char const* name, F func)
    {
        obj_->Set(to_v8(isolate_, name), internal::export_function(isolate_, func));
        return *this;
    }

    // Create an instance of this module in V8
    v8::Local<v8::Object> create_prototype()
    {
        return obj_->NewInstance();
    }

    // v8::Isolate where the module live
    v8::Isolate* isolate()
    {
        return isolate_;
    }

private:
    v8::Isolate* isolate_;
    v8::Handle<v8::ObjectTemplate> obj_;
};

}  // namespace v8cpp
