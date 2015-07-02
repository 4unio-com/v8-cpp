#include "test.h"

#include <v8-cpp.h>

using namespace v8;

void InitAll(Handle<Object> exports)
{
    // Get current isolate
    Isolate* isolate = Isolate::GetCurrent();

    // Prepare TestClass binding
    v8cpp::Class<TestCaller> testcaller(isolate);
    testcaller
            .set_constructor<Local<Function>>()
            .add_method("call_me", &TestCaller::call_me);

    // Prepare module
    v8cpp::Module module(isolate);

    module.add_class("TestCaller", testcaller);

    exports->SetPrototype(module.create_prototype());
}

V8CPP_MODULE(addon, InitAll)
