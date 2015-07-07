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

#include <dlfcn.h>
#include <iostream>

namespace v8cpp
{
namespace internal
{

using ModuleInitFunc = void(v8::Handle<v8::Object> exports);
ModuleInitFunc* node_init_func_;
std::string v8cpp_script_path_;

struct NodeModule
{
    int nm_version;
    unsigned int nm_flags;
    void* nm_dso_handle;
    const char* nm_filename;
    ModuleInitFunc* nm_register_func;
    //...
};

extern "C" void node_module_register(void* m)
{
    auto mp = static_cast<NodeModule*>(m);

    // For now we only know that version 14 works here
    if (mp->nm_version == 14)
    {
        node_init_func_ = mp->nm_register_func;
    }
    else
    {
        std::cerr << "node_module_register(): ignoring node module. nm_version " << mp->nm_version << " not supported"
                  << std::endl;
    }
}

class Console
{
public:
    inline static void log(v8::FunctionCallbackInfo<v8::Value> const& args)
    {
        for (int i = 0; i < args.Length(); ++i)
        {
            if (args[i]->IsUint32())
            {
                std::cout << v8cpp::from_v8<unsigned int>(v8::Isolate::GetCurrent(), args[i]);
            }
            else if (args[i]->IsInt32())
            {
                std::cout << v8cpp::from_v8<int>(v8::Isolate::GetCurrent(), args[i]);
            }
            else if (args[i]->IsNumber())
            {
                std::cout << v8cpp::from_v8<float>(v8::Isolate::GetCurrent(), args[i]);
            }
            else if (args[i]->IsBoolean())
            {
                std::cout << v8cpp::from_v8<bool>(v8::Isolate::GetCurrent(), args[i]);
            }
            else if (args[i]->IsString())
            {
                std::cout << v8cpp::from_v8<std::string>(v8::Isolate::GetCurrent(), args[i]);
            }
        }
        std::cout << std::endl;
        std::cout.flush();
    }
};

inline v8::Local<v8::Object> require(std::string const& module_path)
{
    node_init_func_ = nullptr;
    v8::Local<v8::Object> exports = v8::Object::New(v8::Isolate::GetCurrent());

    // Try append ".node" to module_path
    std::string suffixed_module_path = v8cpp_script_path_ + module_path + ".node";
    auto module = dlopen(suffixed_module_path.c_str(), RTLD_LAZY);
    if (!module)
    {
        // Didn't work, now try append ".so" to module_path
        suffixed_module_path = v8cpp_script_path_ + module_path + ".so";
        module = dlopen(suffixed_module_path.c_str(), RTLD_LAZY);
        if (!module)
        {
            // Still didn't work, just try module_path as is
            suffixed_module_path = v8cpp_script_path_ + module_path;
            module = dlopen(suffixed_module_path.c_str(), RTLD_LAZY);
            if (!module)
            {
                std::cerr << "dlopen failed: " << dlerror() << std::endl;
                return exports;
            }
        }
    }

    if (node_init_func_)
    {
        node_init_func_(exports);
    }
    else
    {
        auto v8cpp_init_func = (ModuleInitFunc*)dlsym(module, "init_module");
        if (!v8cpp_init_func)
        {
            std::cerr << "dlsym failed: " << dlerror() << std::endl;
            return exports;
        }

        v8cpp_init_func(exports);
    }

    return exports;
}

}  // namespace internal
}  // namespace v8cpp
