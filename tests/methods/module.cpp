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

#include "test.h"

#include <v8-cpp.h>

using namespace v8;

void InitAll(Handle<Object> exports)
{
    // Get current isolate
    Isolate* isolate = Isolate::GetCurrent();

    // Prepare TestClass binding
    v8cpp::Class<TestClass_OL> testclass(isolate);
    testclass
            .add_inheritance<BaseBaseTestClass>()
            .add_inheritance<BaseTestClass>()
            .add_inheritance<TestClass>()
            .set_constructor()
            .add_method("regular_method", &TestClass::regular_method)
            .add_method("static_method", &TestClass::static_method)
            .add_method("base_method", &BaseTestClass::base_method)
            .add_method("base_base_method", &BaseBaseTestClass::base_base_method)
            .add_method("virtual_method", &TestClass::virtual_method)
            .add_method("overload_method", &TestClass_OL::overload_method);

    // Prepare module
    v8cpp::Module module(isolate);

    module.add_class("TestClass", testclass);

    exports->SetPrototype(module.create_prototype());
}

V8CPP_MODULE(addon, InitAll)
