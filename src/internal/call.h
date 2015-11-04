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

#include <convert.h>
#include <internal/utility/function_traits.h>
#include <internal/utility/index_sequence.h>

#include <functional>

namespace v8cpp
{
namespace internal
{

template <typename F, size_t Offset = 0>
struct CallFromV8Traits
{
    static bool const is_mem_func = std::is_member_function_pointer<F>::value;
    using Arguments = typename FunctionTraits<F>::Arguments;

    static size_t const arg_count = std::tuple_size<Arguments>::value - is_mem_func - Offset;

    template <size_t Index, bool>
    struct tuple_element
    {
        using Type = typename std::tuple_element<Index, Arguments>::type;
    };

    template <size_t Index>
    struct tuple_element<Index, false>
    {
        using Type = void;
    };

    template <size_t Index>
    using ArgType = typename tuple_element<Index + is_mem_func, (Index < arg_count + Offset)>::Type;

    template <size_t Index>
    using ConvertType = typename Convert<ArgType<Index>>::FromType;

    template <size_t Index>
    static ConvertType<Index> args_from_v8(v8::FunctionCallbackInfo<v8::Value> const& args)
    {
        return Convert<ArgType<Index>>::from_v8(args.GetIsolate(), args[Index - Offset]);
    }

    static void check_args(v8::FunctionCallbackInfo<v8::Value> const& args)
    {
        if (args.Length() != arg_count)
        {
            throw std::runtime_error("argument count does not match the corresponding C++ function definition");
        }
    }
};

template <typename F>
using IsolateArgCallTraits = CallFromV8Traits<F, 1>;

template <typename F, size_t Offset = 0>
struct V8ArgsCallTraits : CallFromV8Traits<F, Offset>
{
    template <size_t Index>
    using ArgType = v8::FunctionCallbackInfo<v8::Value> const&;

    template <size_t Index>
    using ConvertType = v8::FunctionCallbackInfo<v8::Value> const&;

    template <size_t Index>
    static v8::FunctionCallbackInfo<v8::Value> const& args_from_v8(v8::FunctionCallbackInfo<v8::Value> const& args)
    {
        return args;
    }

    static void check_args(v8::FunctionCallbackInfo<v8::Value> const&)
    {
    }
};

template <typename F>
using IsolateV8ArgsCallTraits = V8ArgsCallTraits<F, 1>;
 
template <typename F, size_t Offset>
using IsDirectArgs = std::integral_constant<bool,
                                            CallFromV8Traits<F>::arg_count == (Offset + 1) &&
                                                std::is_same<typename CallFromV8Traits<F>::template ArgType<Offset>,
                                                             v8::FunctionCallbackInfo<v8::Value> >::value>;

template <typename F>
using IsFirstArgIsolate =
    std::integral_constant<bool,
                           CallFromV8Traits<F>::arg_count != 0 &&
                               std::is_same<typename CallFromV8Traits<F>::template ArgType<0>, v8::Isolate*>::value>;

template <typename F>
using SelectCallTraits = typename std::conditional<
    IsFirstArgIsolate<F>::value,
    typename std::conditional<IsDirectArgs<F, 1>::value, IsolateV8ArgsCallTraits<F>, IsolateArgCallTraits<F>>::type,
    typename std::conditional<IsDirectArgs<F, 0>::value, V8ArgsCallTraits<F>, CallFromV8Traits<F>>::type>::type;

template <typename F, typename CallTraits, size_t... Indices>
typename FunctionTraits<F>::ReturnType call_from_v8_impl(F&& func,
                                                         v8::FunctionCallbackInfo<v8::Value> const& args,
                                                         CallTraits,
                                                         IndexSequence<Indices...>)
{
    return func(CallTraits::template args_from_v8<Indices>(args)...);
}

template <typename F, size_t... Indices>
typename FunctionTraits<F>::ReturnType call_from_v8_impl(F&& func,
                                                         v8::FunctionCallbackInfo<v8::Value> const& args,
                                                         IsolateV8ArgsCallTraits<F>,
                                                         IndexSequence<Indices...>)
{
    return func(args.GetIsolate(), IsolateV8ArgsCallTraits<F>::template args_from_v8<Indices>(args)...);
}

template <typename T, typename F, typename CallTraits, size_t... Indices>
typename FunctionTraits<F>::ReturnType call_from_v8_impl(
    T& obj, F&& func, v8::FunctionCallbackInfo<v8::Value> const& args, CallTraits, IndexSequence<Indices...>)
{
    return (obj.*func)(CallTraits::template args_from_v8<Indices>(args)...);
}

template <typename F, size_t... Indices>
typename FunctionTraits<F>::ReturnType call_from_v8_impl(F&& func,
                                                         v8::FunctionCallbackInfo<v8::Value> const& args,
                                                         IsolateArgCallTraits<F>,
                                                         IndexSequence<Indices...>)
{
    return func(args.GetIsolate(), IsolateArgCallTraits<F>::template args_from_v8<Indices + 1>(args)...);
}

template <typename F>
typename FunctionTraits<F>::ReturnType call_from_v8(F&& func, v8::FunctionCallbackInfo<v8::Value> const& args)
{
    using CallTraits = SelectCallTraits<F>;
    CallTraits::check_args(args);
    return call_from_v8_impl(std::forward<F>(func), args, CallTraits(), GenIndexSequence<CallTraits::arg_count>());
}

template <typename T, typename F>
typename FunctionTraits<F>::ReturnType call_from_v8(T& obj, F&& func, v8::FunctionCallbackInfo<v8::Value> const& args)
{
    using CallTraits = SelectCallTraits<F>;
    CallTraits::check_args(args);
    return call_from_v8_impl(obj, std::forward<F>(func), args, CallTraits(), GenIndexSequence<CallTraits::arg_count>());
}

}  // namespace internal
}  // namespace v8cpp
