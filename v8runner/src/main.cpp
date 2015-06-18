#include <v8-cpp.h>

#include <dlfcn.h>
#include <fstream>
#include <iostream>

using namespace v8;

extern "C" void node_module_register(void*) {}
using InitFunc = void(Handle<Object> exports);

Local<Object> require(std::string const& module_path)
{
    auto module = dlopen(module_path.c_str(), RTLD_LAZY);
    if (!module)
    {
        std::cerr << "dlopen failed: " << dlerror() << std::endl;
    };
    auto init_module = (InitFunc*)dlsym(module, "init_module");
    if (!init_module)
    {
        std::cerr << "dlsym failed: " << dlerror() << std::endl;
    };

    Local<Object> exports = Object::New(Isolate::GetCurrent());
    init_module(exports);
    return exports;
}

v8::Handle<v8::Value> run_script(Isolate* isolate, std::string const& source, std::string const& filename)
{
    v8::EscapableHandleScope scope(isolate);

    v8::Local<v8::Script> script = v8::Script::Compile(v8cpp::to_v8(isolate, source),
                                                       v8cpp::to_v8(isolate, filename));

    v8::Local<v8::Value> result;
    if (!script.IsEmpty())
    {
        result = script->Run();
    }
    return scope.Escape(result);
}

v8::Handle<v8::Value> run_script_file(Isolate* isolate, std::string const& filename)
{
    std::ifstream stream(filename.c_str());
    if (!stream)
    {
        throw std::runtime_error("run_script_file(): Failed to locate file: \"" + filename + "\"");
    }

    std::istreambuf_iterator<char> begin(stream), end;
    return run_script(isolate, std::string(begin, end), filename);
}

int main(int argc, char* argv[])
{
    // Check args.
    if (argc != 2)
    {
        std::cerr << "usage: v8runner script.js" << std::endl;
        return -1;
    }

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

        // Run script.
        run_script_file(isolate, argv[1]);
    }

    // Dispose the isolate and tear down V8.
    isolate->Dispose();
    V8::Dispose();
    return 0;
}
