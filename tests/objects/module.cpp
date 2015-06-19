#include <v8-cpp.h>

#include <gtest/gtest.h>

using namespace v8;

class TestClass
{
public:
    TestClass(int i)
    {
        EXPECT_EQ(i, 2);
    }
};

void InitAll(Handle<Object> exports)
{
    // Get current isolate
    Isolate* isolate = Isolate::GetCurrent();

    // Prepare class binding
    v8cpp::Class<TestClass> testclass(isolate);
    testclass
            .set_constructor<int>();

    // Prepare module
    v8cpp::Module module(isolate);

    module.add_class("TestClass", testclass);

    exports->SetPrototype(module.create_prototype());
}

V8CPP_MODULE(addon, InitAll)
