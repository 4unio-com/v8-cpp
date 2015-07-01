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
            .add_method("embedded_class_ptr", &TestClass::embedded_class_ptr)
            .add_method("embedded_class_ref", &TestClass::embedded_class_ref)
            .add_method("embedded_class_copy", &TestClass::embedded_class_copy)
            .add_method("replace_i", &TestClass::replace_i)
            .add_method("add_i", &TestClass::add_i);

    // Prepare EmbeddedTestClass binding
    v8cpp::Class<EmbeddedTestClass> embeddedtestclass(isolate);
    embeddedtestclass
            .set_constructor<int, int>()
            .add_method("i", &EmbeddedTestClass::i);

    // Prepare module
    v8cpp::Module module(isolate);

    module.add_class("TestClass", testclass);
    module.add_class("EmbeddedTestClass", embeddedtestclass);

    module.add_function("new_TestClass", &new_TestClass);

    exports->SetPrototype(module.create_prototype());
}

V8CPP_MODULE(addon, InitAll)
