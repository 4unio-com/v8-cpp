/*
 * Copyright (C) 2015 Canonical Ltd
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 3 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Authored by: Marcus Tomlinson <marcus.tomlinson@canonical.com>
 */

var addon = require('./v8-cpp-example');

var obj = addon.new_MyObject(10);
console.log( obj.plus_one() ); // 11
console.log( obj.plus_one() ); // 12
console.log( obj.plus_one() ); // 13

var obj2 = addon.new_MyObject("1");
console.log( obj2.plus_one() ); // 2
console.log( obj2.plus_one() ); // 3
console.log( obj2.plus_one() ); // 4

console.log( obj.add(obj, obj2) ); // 17

console.log( obj.value() ); // 13
obj.value = function()
{
    return 2;
}
console.log( obj.value() ); // 2

console.log( obj.add(obj, obj2) ); // 17

var obj3 = addon.new_MyObject2(10, 0);
console.log( obj3.plus_two() ); // 12
console.log( obj3.plus_two() ); // 14
console.log( obj3.plus_two() ); // 16
console.log( obj3.plus_two(1) ); // 19
console.log( obj3.plus_two(2) ); // 23
console.log( obj3.plus_two(3) ); // 28

obj3.base_method(); // hello!
addon.MyObject2.static_method(); // static hello!

var s = addon.new_SearchHandler(function(search_string)
{
    console.log(search_string);
});

s.listen(); // hello world

var struct = new addon.MyStruct();
struct.bool_value = true;
struct.int_value = 10;
console.log(struct.string_value); // hello
struct.string_value = "world";
struct.output(); // world

delete struct
