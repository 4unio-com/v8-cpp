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

template <typename T, T... I>
struct IntSequence
{
    template <T N>
    using Append = IntSequence<T, I..., N>;

    using Next = Append<sizeof...(I)>;
};

template <typename T, T Index, size_t N>
struct SequenceGen
{
    using Type = typename SequenceGen<T, Index - 1, N - 1>::Type::Next;
};

template <typename T, T Index>
struct SequenceGen<T, Index, 0ul>
{
    using Type = IntSequence<T>;
};

template <size_t... I>
using IndexSequence = IntSequence<size_t, I...>;

template <typename T, T N>
using GenIntSequence = typename SequenceGen<T, N, N>::Type;

template <size_t N>
using GenIndexSequence = GenIntSequence<size_t, N>;

}  // namespace internal
}  // namespace v8cpp
