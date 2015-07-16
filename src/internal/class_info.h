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

#include <internal/utility/moveable_persistent.h>

#include <assert.h>
#include <algorithm>
#include <unordered_map>

namespace v8cpp
{
namespace internal
{

class ClassInfo
{
public:
    ClassInfo(size_t type)
        : type_(type)
    {
    }

    ClassInfo(ClassInfo const&) = delete;
    ClassInfo& operator=(ClassInfo const&) = delete;

    using CastFunction = void* (*)(void*);

    inline void add_base_class(ClassInfo* info, CastFunction cast)
    {
        auto it = std::find_if(base_classes_.begin(), base_classes_.end(), [info](BaseClassInfo const& base_class)
                               {
                                   return base_class.info == info;
                               });
        if (it == base_classes_.end())
        {
            base_classes_.emplace_back(info, cast);
            info->child_classes_.emplace_back(this);
        }
    }

    inline bool cast(void*& ptr, size_t type) const
    {
        // Type already matches
        if (type == type_)
        {
            return true;
        }

        // Traverse direct parents
        for (BaseClassInfo const& base_class : base_classes_)
        {
            if (base_class.info->type_ == type)
            {
                ptr = base_class.cast(ptr);
                return true;
            }
        }

        // Traverse class hierarchy
        for (BaseClassInfo const& base_class : base_classes_)
        {
            void* p = base_class.cast(ptr);
            if (base_class.info->cast(p, type))
            {
                ptr = p;
                return true;
            }
        }

        return false;
    }

    template <typename T>
    void add_object(T* object, MoveablePersistent<v8::Object>&& handle)
    {
        assert(objects_.find(object) == objects_.end() && "object already exists");
        objects_.emplace(object, std::move(handle));
    }

    template <typename T>
    void remove_object(v8::Isolate* isolate, T* object, void (*destroy)(v8::Isolate*, T*))
    {
        auto it = objects_.find(object);
        assert(objects_.find(object) != objects_.end() && "object does not exist");
        if (it != objects_.end())
        {
            it->second.Reset();
            if (destroy)
            {
                destroy(isolate, object);
            }
            objects_.erase(it);
        }
    }

protected:
    inline static size_t register_class()
    {
        static size_t next_index = 0;
        return next_index++;
    }

private:
    struct BaseClassInfo
    {
        ClassInfo* info;
        CastFunction cast;

        BaseClassInfo(ClassInfo* info, CastFunction cast)
            : info(info)
            , cast(cast)
        {
        }
    };

    size_t type_;
    std::vector<BaseClassInfo> base_classes_;
    std::vector<ClassInfo*> child_classes_;

    std::unordered_map<void*, MoveablePersistent<v8::Object>> objects_;
};

}  // namespace internal
}  // namespace v8cpp
