#include "test.h"

#include <v8-cpp.h>

using namespace v8;

void InitAll(Handle<Object> exports)
{
    // Get current isolate
    Isolate* isolate = Isolate::GetCurrent();

    // Prepare class binding
    v8cpp::Class<TestClass> testclass(isolate);
    testclass
            .set_constructor<int, int>();

    // Prepare module
    v8cpp::Module module(isolate);

    module.add_class("TestClass", testclass);
    module.add_function("new_TestClass", &new_TestClass);

    exports->SetPrototype(module.create_prototype());
}

V8CPP_MODULE(addon, InitAll)
