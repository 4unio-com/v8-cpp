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
