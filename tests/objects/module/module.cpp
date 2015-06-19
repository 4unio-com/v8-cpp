#include <v8-cpp.h>

using namespace v8;

void InitAll(Handle<Object> exports)
{
}

V8CPP_MODULE(addon, InitAll)
