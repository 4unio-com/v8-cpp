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

#include <internal/class_info.h>
#include <internal/function.h>
#include <internal/utility/object_factory.h>

namespace v8cpp
{
namespace internal
{

template <typename T>
class Class : public ClassInfo
{
public:
    static Class& instance(v8::Isolate* isolate)
    {
        // Get all Class instances from isolate
        using ClassInstances = std::vector<Class*>;
        ClassInstances* instances = static_cast<ClassInstances*>(isolate->GetData(0));
        if (!instances)
        {
            // There are no Class instances yet so we create it here
            instances = new ClassInstances;
            isolate->SetData(0, instances);
        }

        // Find Class instance by class type
        size_t my_type = class_type_();
        Class* result;
        if (my_type < instances->size())
        {
            result = static_cast<Class*>((*instances)[my_type]);
        }
        else
        {
            // There is no Class instance yet so we create it here
            result = new Class(isolate, my_type);
            instances->emplace_back(result);
        }
        return *result;
    }

    v8::Isolate* isolate()
    {
        return isolate_;
    }

    v8::Local<v8::FunctionTemplate> class_template()
    {
        return to_local(isolate_, class_template_);
    }

    v8::Local<v8::FunctionTemplate> proto_template()
    {
        return to_local(isolate_, proto_template_.IsEmpty() ? class_template_ : proto_template_);
    }

    template <typename... Args>
    void set_constructor()
    {
        constructor_ = [](v8::FunctionCallbackInfo<v8::Value> const& args)
        {
            using CtorType = T* (*)(v8::Isolate*, Args...);
            return call_from_v8(static_cast<CtorType>(&ObjectFactory<T>::new_object), args);
        };
        class_template()->Inherit(proto_template());
    }

    template <typename U>
    void add_inheritance()
    {
        Class<U>* base_class = &Class<U>::instance(isolate_);
        add_base_class(base_class, [](void* ptr) -> void*
                                   {
                                       return static_cast<U*>(static_cast<T*>(ptr));
                                   });
        proto_template()->Inherit(base_class->class_template());
    }

    v8::Local<v8::Object> export_object(T* object)
    {
        v8::EscapableHandleScope scope(isolate_);

        v8::Local<v8::Object> v8_object = class_template()->GetFunction()->NewInstance();
        v8_object->SetAlignedPointerInInternalField(0, object);
        v8_object->SetAlignedPointerInInternalField(1, this);

        MoveablePersistent<v8::Object> v8_object_p(isolate_, v8_object);
        v8_object_p.SetWeak(object, [](v8::WeakCallbackData<v8::Object, T> const& data)
                            {
                                v8::Isolate* isolate = data.GetIsolate();
                                T* object = data.GetParameter();
                                instance(isolate).destroy_object(object);
                            });

        ClassInfo::add_object(object, std::move(v8_object_p));

        return scope.Escape(v8_object);
    }

    v8::Local<v8::Object> export_object(T const* object)
    {
        return export_object(const_cast<T*>(object));
    }

    v8::Local<v8::Object> export_object(v8::FunctionCallbackInfo<v8::Value> const& args)
    {
        return constructor_ ? export_object(constructor_(args)) :
                              throw std::runtime_error("exported class does not have a constructor specified");
    }

    T* import_object(v8::Local<v8::Value> value)
    {
        v8::HandleScope scope(isolate_);

        while (value->IsObject())
        {
            v8::Local<v8::Object> object = value->ToObject();
            if (object->InternalFieldCount() == 2)
            {
                void* ptr = object->GetAlignedPointerFromInternalField(0);
                ClassInfo* info = static_cast<ClassInfo*>(object->GetAlignedPointerFromInternalField(1));
                if (info && info->cast(ptr, class_type_()))
                {
                    return static_cast<T*>(ptr);
                }
            }
            value = object->GetPrototype();
        }
        return nullptr;
    }

    void destroy_object(T* object)
    {
        ClassInfo::remove_object(isolate_, object, &ObjectFactory<T>::delete_object);
    }

    template <typename P>
    static void get_member(v8::Local<v8::String>, v8::PropertyCallbackInfo<v8::Value> const& info)
    {
        v8::Isolate* isolate = info.GetIsolate();

        T const& self = from_v8<T const&>(isolate, info.This());
        P property = import_value<P>(info.Data());
        info.GetReturnValue().Set(to_v8(isolate, self.*property));
    }

    template <typename P>
    static void set_member(v8::Local<v8::String>,
                           v8::Local<v8::Value> value,
                           v8::PropertyCallbackInfo<void> const& info)
    {
        v8::Isolate* isolate = info.GetIsolate();

        T& self = from_v8<T&>(isolate, info.This());
        P property_ptr = import_value<P>(info.Data());
        using PropType = typename FunctionTraits<P>::ReturnType;
        self.*property_ptr = from_v8<PropType>(isolate, value);
    }

private:
    explicit Class(v8::Isolate* isolate, size_t type)
        : ClassInfo(type)
        , isolate_(isolate)
        , constructor_(nullptr)
    {
        v8::Local<v8::FunctionTemplate> class_template = v8::FunctionTemplate::New(isolate_);
        v8::Local<v8::FunctionTemplate> proto_template = v8::FunctionTemplate::New(
            isolate_, [](v8::FunctionCallbackInfo<v8::Value> const& args)
            {
                v8::Isolate* isolate = args.GetIsolate();
                try
                {
                    return args.GetReturnValue().Set(instance(isolate).export_object(args));
                }
                catch (std::exception const& e)
                {
                    auto exception =
                        isolate->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(isolate, e.what())));
                    args.GetReturnValue().Set(exception);
                }
            });

        class_template_.Reset(isolate_, class_template);
        proto_template_.Reset(isolate_, proto_template);

        // Each JS instance has a built-in field holding a pointer to the C++ object
        // We add a second field to hold a pointer to this Class instance
        class_template->InstanceTemplate()->SetInternalFieldCount(2);
    }

    static size_t class_type_()
    {
        static size_t my_type = ClassInfo::register_class();
        return my_type;
    }

    v8::Isolate* isolate_;
    std::function<T*(v8::FunctionCallbackInfo<v8::Value> const& args)> constructor_;

    v8::UniquePersistent<v8::FunctionTemplate> class_template_;
    v8::UniquePersistent<v8::FunctionTemplate> proto_template_;
};

}  // namespace internal
}  // namespace v8cpp
