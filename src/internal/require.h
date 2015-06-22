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

using InitFunc = void(v8::Handle<v8::Object> exports);
InitFunc* node_init_func;

struct NodeModule
{
  int nm_version;
  unsigned int nm_flags;
  void* nm_dso_handle;
  const char* nm_filename;
  InitFunc* nm_register_func;
  //...
};

extern "C" void node_module_register(void* m)
{
    auto mp = static_cast<NodeModule*>(m);

    // For now we only know that version 14 works here
    if (mp->nm_version == 14)
    {
        node_init_func = mp->nm_register_func;
    }
    else
    {
        std::cerr << "node_module_register(): ignoring node module. nm_version " << mp->nm_version
                  << " not supported" << std::endl;
    }
}

inline v8::Local<v8::Object> require(std::string module_path)
{
    v8::Local<v8::Object> exports = v8::Object::New(v8::Isolate::GetCurrent());

    std::string suffix = ".node";
    if (module_path.size() >= suffix.size() &&
        module_path.compare(module_path.size() - suffix.size(), suffix.size(), suffix) != 0)
    {
        module_path += suffix;
    }

    node_init_func = nullptr;

    auto module = dlopen(module_path.c_str(), RTLD_LAZY);
    if (!module)
    {
        std::cerr << "dlopen failed: " << dlerror() << std::endl;
        return exports;
    }

    if (node_init_func)
    {
        node_init_func(exports);
    }
    else
    {
        auto v8cpp_init_func = (InitFunc*)dlsym(module, "init_module");
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
