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

uv_async_t async;

void print_progress(uv_async_t *handle, int status) {
    double percentage = *((double*) handle->data);
    fprintf(stderr, "Downloaded %.2f%%\n", percentage);
}

void fake_download(uv_work_t *req) {
    int size = *((int*) req->data);
    int downloaded = 0;
    double percentage;
    while (downloaded < size) {
        percentage = downloaded*100.0/size;
        async.data = (void*) &percentage;
        uv_async_send(&async);

        downloaded += (200+random())%1000; // can only download max 1000bytes/sec,
                                           // but at least a 200;
    }
}

void after(uv_work_t *req, int status) {
    fprintf(stderr, "Download complete\n");
    uv_close((uv_handle_t*) &async, NULL);
}

class TestCaller
{
public:
    explicit TestCaller(v8::Local<v8::Function> const& cb)
        : cb_(v8::Isolate::GetCurrent(), cb)
    {
    }

    std::string call_me() const
    {
        uv_loop_t* loop = uv_default_loop();

        uv_work_t req;
        int size = 10240;
        req.data = (void*) &size;

        uv_async_init(loop, &async, print_progress);
        uv_queue_work(loop, &req, fake_download, after);

        uv_run(loop, UV_RUN_DEFAULT);

        v8::Local<v8::Function> cb = v8cpp::to_local<v8::Function>(v8::Isolate::GetCurrent(), cb_);
        auto result = v8cpp::call_v8(v8::Isolate::GetCurrent(), cb, "hello");
        return v8cpp::from_v8<std::string>(v8::Isolate::GetCurrent(), result);
    }

private:
    v8::Persistent<v8::Function> cb_;
};

std::string simple_function()
{
    return "hello there";
}

std::vector<float> complex_function(int first, std::string const& second, float third, bool forth)
{
    EXPECT_EQ(first, 4);
    EXPECT_EQ(second, "3");
    EXPECT_FLOAT_EQ(third, 2.1);
    EXPECT_EQ(forth, false);

    std::vector<float> num_list;
    num_list.push_back(first);
    num_list.push_back(stoi(second));
    num_list.push_back(third);
    num_list.push_back(forth);
    return num_list;
}
