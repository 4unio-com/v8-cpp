#include <v8-cpp.h>

#include <dlfcn.h>

using namespace v8;

extern "C" void node_module_register(void*) {}

using InitFunc = void(Handle<Object> exports);

Local<Object> require(std::string const& module)
{
    auto handle = dlopen(module.c_str(), RTLD_LAZY);
    if (!handle)
    {
        fprintf(stderr, "dlopen failed: %s\n", dlerror());
    };
    auto sym = (InitFunc*)dlsym(handle, "init_module");
    if (!sym)
    {
        fprintf(stderr, "dlsym failed: %s\n", dlerror());
    };

    Local<Object> exports = Object::New(Isolate::GetCurrent());
    sym(exports);
    return exports;
}

int main(int argc, char* argv[])
{
    // Initialize V8.
    V8::Initialize();

    // Create a new Isolate and make it the current one.
    Isolate* isolate = Isolate::New();
    {
        Isolate::Scope isolate_scope(isolate);

        // Create a stack-allocated handle scope.
        HandleScope handle_scope(isolate);

        // Create a new context.
        v8cpp::Module module(isolate);
        module.add_function("require", &require);
        Local<Context> context = Context::New(isolate, nullptr, module.object_template());

        // Enter the context for compiling and running the hello world script.
        Context::Scope context_scope(context);

        // Create a string containing the JavaScript source code.
        Local<String> source = String::NewFromUtf8(isolate,
        R"(
            var addon = require("/home/marcustomlinson/Projects/work/v8-cpp/v8runner-build/test/v8-cpp-test.node");

            var obj = addon.new_MyObject2(10, 0);

            obj.base_method(); // hello!
            addon.MyObject2.static_method(); // static hello!

            var s = addon.new_SearchHandler(function(search_string)
            {
                obj.base_method();
            });

            s.listen(); // hello!

            var struct = new addon.MyStruct();
            struct.bool_value = true;
            struct.int_value = 10;
            struct.string_value = "world";
            struct.output(); // world

            delete struct
        )");

        // Compile the source code.
        Local<Script> script = Script::Compile(source);

        // Run the script.
        script->Run();
    }

    // Dispose the isolate and tear down V8.
    isolate->Dispose();
    V8::Dispose();
    return 0;
}
