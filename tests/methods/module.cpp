#include "test.h"

#include <v8-cpp.h>

using namespace v8;

void InitAll(Handle<Object> exports)
{
    // Get current isolate
    Isolate* isolate = Isolate::GetCurrent();

    // Prepare TestClass binding

    // Prepare module
    v8cpp::Module module(isolate);

    exports->SetPrototype(module.create_prototype());
}

V8CPP_MODULE(addon, InitAll)
