//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP___TUPLE_GET_ELEMENT_H
#define _LIBCPP___TUPLE_GET_ELEMENT_H

#include <__config>
#include <__utility/forward.h>
#include <cstddef>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#  pragma GCC system_header
#endif

_LIBCPP_BEGIN_NAMESPACE_STD

#if _LIBCPP_STD_VER >= 20

namespace __detail
{
template <std::size_t _Ip>
struct __get_element_fn
{
    template <typename _TupleLike>
    auto operator()(_TupleLike&& __tuple_like_object) const
        -> decltype(get<_Ip>(std::forward<_TupleLike>(__tuple_like_object)))
    {
        return get<_Ip>(std::forward<_TupleLike>(__tuple_like_object));
    }
}; // struct __get_element_fn
} // namespace __detail

inline namespace __get_element_namespace
{
template <std::size_t _Ip>
inline constexpr __detail::__get_element_fn<_Ip> get_element;
}

inline constexpr auto get_key = get_element<0>;
inline constexpr auto get_value = get_element<1>;

#endif // _LIBCPP_STD_VER

_LIBCPP_END_NAMESPACE_STD

#endif // _LIBCPP___TUPLE_GET_ELEMENT_H
