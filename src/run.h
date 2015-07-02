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

#include <internal/require.h>

#include <fstream>

namespace v8cpp
{

template <typename T = v8::Handle<v8::Value>>
T run_script(v8::Isolate* isolate, std::string const& source, std::string const& filename = "")
{
    // Create an isolate scope.
    v8::Isolate::Scope isolate_scope(isolate);

    // Create a stack-allocated handle scope.
    v8::HandleScope handle_scope(isolate);

    // Prepare console class
    v8cpp::Class<internal::Console> console(isolate);
    console
            .set_constructor()
            .add_method("log", &internal::Console::log);

    // Create a new context that supports "require()" and "console".
    v8cpp::Module module(isolate);
    {
        v8::Context::Scope context_scope(v8::Context::New(isolate));

        module.add_function("require", &internal::require);
        module.add_class("console", console);
    }
    v8::Local<v8::Context> context = v8::Context::New(isolate, nullptr, module.object_template());

    // Enter the context for compiling and running the script.
    v8::Context::Scope context_scope(context);

    // Compile the script.
    v8::Local<v8::Script> script = v8::Script::Compile(v8cpp::to_v8(isolate, source), v8cpp::to_v8(isolate, filename));

    // Run the script.
    if (script.IsEmpty())
    {
        if (!filename.empty())
        {
            throw std::runtime_error("run_script(): Failed to compile script file: \"" + filename + "\"");
        }
        else
        {
            throw std::runtime_error("run_script(): Failed to compile script.");
        }
    }

    return v8cpp::from_v8<T>(isolate, script->Run());
}

template <typename T = v8::Handle<v8::Value>>
T run_script_file(v8::Isolate* isolate, std::string const& filename)
{
    std::ifstream stream(filename.c_str());
    if (!stream)
    {
        throw std::runtime_error("run_script_file(): Failed to locate script file: \"" + filename + "\"");
    }

    std::istreambuf_iterator<char> begin(stream), end;
    return run_script<T>(isolate, std::string(begin, end), filename);
}
}
