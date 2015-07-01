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

template <typename F>
struct FunctionTraits;

template <typename R, typename... Args>
struct FunctionTraits<R(Args...)>
{
    using ReturnType = R;
    using Arguments = std::tuple<Args...>;
};

// Function pointer
template <typename R, typename... Args>
struct FunctionTraits<R (*)(Args...)> : FunctionTraits<R(Args...)>
{
};

// Member function pointer
template <typename C, typename R, typename... Args>
struct FunctionTraits<R (C::*)(Args...)> : FunctionTraits<R(C&, Args...)>
{
};

// Const member function pointer
template <typename C, typename R, typename... Args>
struct FunctionTraits<R (C::*)(Args...) const> : FunctionTraits<R(C const&, Args...)>
{
};

// Member object pointer
template <typename C, typename R>
struct FunctionTraits<R(C::*)> : FunctionTraits<R(C&)>
{
};

}  // namespace internal
}  // namespace v8cpp
