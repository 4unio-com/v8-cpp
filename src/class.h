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

#include <internal/class.h>

#include <v8.h>

namespace v8cpp
{

// API for exporting C++ classes to V8 (abstraction of v8::FunctionTemplate)
template <typename T>
class Class
{
public:
    explicit Class(v8::Isolate* isolate)
        : class_(internal::Class<T>::instance(isolate))
    {
    }

    // Set class constructor
    template <typename... Args>
    Class& set_constructor()
    {
        class_.set_constructor<Args...>();
        return *this;
    }

    // Inhert from class
    template <typename U>
    Class& add_inheritance()
    {
        static_assert(std::is_base_of<U, T>::value, "Class U is not a base of class T");
        class_.add_inheritance<U>();
        return *this;
    }

    // Add class method
    template <typename M>
    typename std::enable_if<std::is_member_function_pointer<M>::value, Class&>::type add_method(char const* name,
                                                                                                M mem_func)
    {
        class_.class_template()->PrototypeTemplate()->Set(class_.isolate(), name,
                                                          internal::export_function(class_.isolate(), mem_func));
        return *this;
    }

    // Add static class method
    template <typename F>
    typename std::enable_if<internal::IsFunctionPointer<F>::value, Class&>::type add_method(char const* name, F func)
    {
        class_.proto_template()->Set(class_.isolate(), name, internal::export_function(class_.isolate(), func));
        return *this;
    }

    // Add class member
    template <typename P>
    Class& add_member(char const* name, P property, bool readonly = false)
    {
        v8::HandleScope scope(class_.isolate());

        v8::AccessorGetterCallback getter = &internal::Class<T>::template get_member<P>;
        v8::AccessorSetterCallback setter = &internal::Class<T>::template set_member<P>;
        if (readonly)
        {
            setter = nullptr;
        }

        v8::Handle<v8::Value> data = internal::export_value(class_.isolate(), property);
        v8::PropertyAttribute prop_attrs = v8::PropertyAttribute(v8::DontDelete | (setter ? 0 : v8::ReadOnly));

        class_.class_template()->PrototypeTemplate()->SetAccessor(to_v8(class_.isolate(), name), getter, setter, data,
                                                                  v8::DEFAULT, prop_attrs);
        return *this;
    }

private:
    friend class Module;
    internal::Class<T>& class_;
};

}  // namespace v8cpp
