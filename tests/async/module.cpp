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

#include <unistd.h>
#include <string.h>

#include <v8-cpp.h>
#include <uv.h>

using namespace v8;

unsigned long long count = 0;

// native blocking/compute intensive function
void delay(int seconds) {
    int i;
    int j;

    // a long computation
    for(i=0;i<2000000;++i) {
        for(j=0;j<400;++j)   {
                count = count * seconds;
        }
    }

    /**
     * or a blocking call
     * sleep(seconds);
     */
}

// the 'baton' is the carrier for data between functions
struct DelayBaton
{
    explicit DelayBaton(v8::Local<v8::Function> const& cb)
        : callback(v8::Isolate::GetCurrent(), cb) {}

    // required
    uv_work_t request;                  // libuv
    Persistent<Function> callback;      // javascript callback

    // optional : data goes here.
    // data that doesn't go back to javascript can be any typedef
    // data that goes back to javascript needs to be a supported type
    int         seconds;
    char        greeting[256];
};

// called by libuv worker in separate thread
static void DelayAsync(uv_work_t *req)
{
    DelayBaton *baton = static_cast<DelayBaton *>(req->data);
    delay(baton->seconds);
}

// called by libuv in event loop when async function completes
static void DelayAsyncAfter(uv_work_t *req,int status)
{
    // get the reference to the baton from the request
    DelayBaton *baton = static_cast<DelayBaton *>(req->data);

    // execute the callback
    v8::Local<v8::Function> cb = v8cpp::to_local<v8::Function>(v8::Isolate::GetCurrent(), baton->callback);
    v8cpp::call_v8(v8::Isolate::GetCurrent(), cb, baton->seconds, baton->greeting);

    // dispose the callback object from the baton
    //baton->callback.Dispose();

    // delete the baton object
    delete baton;
}

// javascript callable function
Handle<Value> Delay(FunctionCallbackInfo<Value> const& args)
{
    // create 'baton' data carrier
    DelayBaton *baton = new DelayBaton(Handle<Function>::Cast(args[2]));

    // attach baton to uv work request
    baton->request.data = baton;

    // assign incoming arguments to baton
    baton->seconds =   args[0]->Int32Value();

    // point at the argument as a string, then copy it to the baton
    v8::String::Utf8Value str(args[1]);
    strncpy(baton->greeting,*str,sizeof(baton->greeting));

    // queue the async function to the event loop
    // the uv default loop is the node.js event loop
    uv_queue_work(uv_default_loop(),&baton->request,DelayAsync,DelayAsyncAfter);

    // nothing returned
    return Undefined(v8::Isolate::GetCurrent());
}

void InitAll(Handle<Object> exports) {
  // Get current isolate
  Isolate* isolate = Isolate::GetCurrent();

  // Prepare module
  v8cpp::Module module(isolate);

  module.add_function("delay", &Delay);

  exports->SetPrototype(module.create_prototype());
}

V8CPP_MODULE(addon, InitAll)
