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

namespace v8cpp
{
namespace internal
{

// Moveable v8::UniquePersistent
template <typename T>
struct MoveablePersistent : public v8::UniquePersistent<T>
{
    using BaseClass = v8::UniquePersistent<T>;
    using BaseClass::BaseClass;

    MoveablePersistent(MoveablePersistent&& src)
        : BaseClass(src.Pass())
    {
    }

    MoveablePersistent(MoveablePersistent const&) = delete;
};

}  // namespace internal
}  // namespace v8cpp
