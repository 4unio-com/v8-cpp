#include "test.h"

#include <v8-cpp.h>

using namespace v8;

void InitAll(Handle<Object> exports)
{
    // Get current isolate
    Isolate* isolate = Isolate::GetCurrent();

    // Prepare TestStruct binding
    v8cpp::Class<TestStruct> teststruct(isolate);
    teststruct
            .set_constructor()
            .add_member("bool_value", &TestStruct::bool_value)
            .add_member("int_value", &TestStruct::int_value)
            .add_member("float_value", &TestStruct::float_value)
            .add_member("string_value", &TestStruct::string_value);

    // Prepare module
    v8cpp::Module module(isolate);

    module.add_class("TestStruct", teststruct);

    exports->SetPrototype(module.create_prototype());
}

V8CPP_MODULE(addon, InitAll)
