#include "include/v8.h"

#include <dlfcn.h>

using namespace v8;

extern "C" void node_module_register(void*) {}

using InitFunc = void(Handle<Object> exports);

int main(int argc, char* argv[])
{
    auto handle = dlopen("/home/marcustomlinson/Projects/work/v8-cpp/v8runner-build/test/v8-cpp-test.node", RTLD_LAZY);
    if (!handle)
    {
        fprintf(stderr, "dlopen failed: %s\n", dlerror());
    };
    auto sym = (InitFunc*)dlsym(handle, "init_module");
    if (!sym)
    {
        fprintf(stderr, "dlsym failed: %s\n", dlerror());
    };

    // Initialize V8.
    V8::Initialize();

    // Create a new Isolate and make it the current one.
    Isolate* isolate = Isolate::New();
    {
        Isolate::Scope isolate_scope(isolate);

        // Create a stack-allocated handle scope.
        HandleScope handle_scope(isolate);

        // Create a new context.
        Local<Context> context = Context::New(isolate);

        // Enter the context for compiling and running the hello world script.
        Context::Scope context_scope(context);

        Local<Object> exports = Object::New(isolate);
        sym(exports);

        // Create a string containing the JavaScript source code.
        Local<String> source = String::NewFromUtf8(isolate,
        R"(
            "hello world"
        )");

        // Compile the source code.
        Local<Script> script = Script::Compile(source);

        // Run the script to get the result.
        Local<Value> result = script->Run();

        // Convert the result to an UTF8 string and print it.
        String::Utf8Value utf8(result);
        printf("%s\n", *utf8);
    }

    // Dispose the isolate and tear down V8.
    isolate->Dispose();
    V8::Dispose();
    return 0;
}
