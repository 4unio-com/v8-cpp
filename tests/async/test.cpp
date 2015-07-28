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

#include "test.h"

#include <v8-cpp.h>

#include <gtest/gtest.h>

// ASYNC
// call JS function from C++ (callback)
// call C++ functions from JS

TEST(Test, call_to_js)
{
    auto callback_result = v8cpp::run_script(
    R"(
            var addon = require('./test-async-module');

            var i = 0;

            //setInterval(function() {
            //  console.log(i++);
            //},500);

            // test the delay function
            addon.delay(3,'hello world',function(a,b) {
              console.log('delay : ' + a + ',' + b);
            });
    )");
}
