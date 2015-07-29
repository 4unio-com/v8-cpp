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

#include <memory>

namespace v8cpp
{
namespace internal
{

class Locker
{
public:
    explicit Locker(v8::Isolate* isolate)
    {
        if (v8::Locker::IsActive() && !v8::Locker::IsLocked(isolate))
        {
            locker_ = std::make_shared<v8::Locker>(isolate);
            isolate_scope_ = std::make_shared<v8::Isolate::Scope>(isolate);
            handle_scope_ = std::make_shared<v8::HandleScope>(isolate);
            context_scope_ = std::make_shared<v8::Context::Scope>(v8::Context::New(isolate));
        }
    }

private:
    std::shared_ptr<v8::Locker> locker_;
    std::shared_ptr<v8::Isolate::Scope> isolate_scope_;
    std::shared_ptr<v8::HandleScope> handle_scope_;
    std::shared_ptr<v8::Context::Scope> context_scope_;
};

}  // namespace internal
}  // namespace v8cpp
