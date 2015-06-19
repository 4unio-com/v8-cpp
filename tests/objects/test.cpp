#include <gtest/gtest.h>

#include <v8-cpp.h>

// OBJECTS
// construct class via new() from JS
// construct class via factory methods from JS (constructor overloads)
// move/ref objects to JS
// move/ref objects back to C++
// delete class from JS

// METHODS
// call regular class methods from JS
// call static class methods from JS
// call base class methods from JS (inheritance)
// call overridden class methods from JS (inheritance)
// call overloaded class methods from JS
// call template methods from JS??

// MEMBERS
// get/set class/struct members from JS

// FUNCTIONS
// call JS function from C++ (callback)
// call C++ functions from JS

TEST(Test, basic)
{
    v8::Isolate* isolate = v8::Isolate::New();

    v8cpp::run_script(isolate,
    R"(
        var module = require("./module/test-objects-module");
        var x = new module.TestClass(2);
    )");
}
