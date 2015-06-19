var addon = require("../../test/v8-cpp-test");

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
