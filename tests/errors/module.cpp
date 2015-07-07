#include "test.h"

#include <v8-cpp.h>

using namespace v8;

void InitAll(Handle<Object> exports)
{
    // Get current isolate
    Isolate* isolate = Isolate::GetCurrent();

    // Prepare TestClass binding
    v8cpp::Class<TestClass> testclass(isolate);
    testclass
            .set_constructor<int, int>()
            .add_method("i", &TestClass::i)
            .add_method("throw_ex", &TestClass::throw_ex);

    // Prepare module
    v8cpp::Module module(isolate);

    module.add_class("TestClass", testclass);

    exports->SetPrototype(module.create_prototype());
}

V8CPP_MODULE(addon, InitAll)