#include <v8-cpp.h>

using namespace v8;

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
    v8cpp::run_script_file(isolate, argv[1]);

    // Clean up.
    isolate->Dispose();

    return 0;
}
