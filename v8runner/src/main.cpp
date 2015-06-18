#include <v8-cpp.h>

#include <dlfcn.h>
#include <fstream>
#include <iostream>

using namespace v8;

extern "C" void node_module_register(void*) {}
using InitFunc = void(Handle<Object> exports);

Local<Object> require(std::string const& module_path)
{
    Local<Object> exports = Object::New(Isolate::GetCurrent());

    auto module = dlopen(module_path.c_str(), RTLD_LAZY);
    if (!module)
    {
        std::cerr << "dlopen failed: " << dlerror() << std::endl;
        return exports;
    }

    auto init_module = (InitFunc*)dlsym(module, "init_module");
    if (!init_module)
    {
        std::cerr << "dlsym failed: " << dlerror() << std::endl;
        return exports;
    }

    init_module(exports);
    return exports;
}

void run_script(Isolate* isolate, std::string const& source, std::string const& filename)
{
    // Create an isolate scope.
    Isolate::Scope isolate_scope(isolate);

    // Create a stack-allocated handle scope.
    HandleScope handle_scope(isolate);

    // Create a new context that supports "require()".
    v8cpp::Module module(isolate);
    module.add_function("require", &require);
    Local<Context> context = Context::New(isolate, nullptr, module.object_template());

    // Enter the context for compiling and running the script.
    Context::Scope context_scope(context);

    // Compile the script.
    Local<Script> script = Script::Compile(v8cpp::to_v8(isolate, source),
                                           v8cpp::to_v8(isolate, filename));

    // Run the script.
    if (script.IsEmpty())
    {
        std::cerr << "run_script(): Failed to compile script file: \"" << filename << "\"" << std::endl;
        return;
    }

    script->Run();
}

void run_script_file(Isolate* isolate, std::string const& filename)
{
    std::ifstream stream(filename.c_str());
    if (!stream)
    {
        std::cerr << "run_script_file(): Failed to locate script file: \"" << filename << "\"" << std::endl;
        return;
    }

    std::istreambuf_iterator<char> begin(stream), end;
    run_script(isolate, std::string(begin, end), filename);
}

int main(int argc, char* argv[])
{
    // Check args.
    if (argc != 2)
    {
        std::cerr << "usage: v8runner script.js" << std::endl;
        return -1;
    }

    // Create a new isolate to run our script in.
    Isolate* isolate = Isolate::New();

    // Run script.
    run_script_file(isolate, argv[1]);

    return 0;
}
