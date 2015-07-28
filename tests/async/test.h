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

#pragma once

#include <gtest/gtest.h>

#include <v8-cpp.h>
#include <thread>
#include <uv.h>
#include <node.h>

void print_progress(uv_async_t* handle)
{
    double percentage = *((double*)handle->data);
    fprintf(stderr, "Downloaded %.2f%%\n", percentage);
}

void run_scope(uv_work_t* req)
{
    int size = 10240;

    uv_async_t* async = (uv_async_t*)req->data;

    int downloaded = 0;
    double percentage;
    while (downloaded < size)
    {
        percentage = downloaded * 100.0 / size;

        // Execute async function in node main loop
        async->data = (void*)&percentage;
        uv_async_send(async);

        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        downloaded += (200 + random()) % 1000;  // can only download max 1000bytes/sec,
        // but at least a 200;
    }
}

void after(uv_work_t* req, int status)
{
    uv_async_t* async = (uv_async_t*)req->data;

    fprintf(stderr, "Download complete\n");
    uv_close((uv_handle_t*)async, NULL);
}

void Do()
{
    uv_async_t async;
    uv_async_init(uv_default_loop(), &async, print_progress);

    uv_work_t work;
    work.data = (void*)&async;

    // Run work in seperate v8 thread
    uv_queue_work(uv_default_loop(), &work, run_scope, after);

    // Process all pending tasks on the event queue
    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
}
