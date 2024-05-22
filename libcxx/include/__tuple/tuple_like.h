//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP___TUPLE_TUPLE_LIKE_H
#define _LIBCPP___TUPLE_TUPLE_LIKE_H

#include <__config>
#include <__fwd/subrange.h>
#include <__tuple/tuple_like_no_subrange.h>
#include <__tuple/tuple_size.h>
#include <__tuple/tuple_element.h>
#include <__tuple/get_element.h>
#include <__type_traits/remove_cvref.h>
#include <__type_traits/is_reference.h>
#include <__utility/integer_sequence.h>
#include <__concepts/convertible_to.h>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#  pragma GCC system_header
#endif

_LIBCPP_BEGIN_NAMESPACE_STD

#if _LIBCPP_STD_VER >= 20

template <class _Tp>
inline constexpr bool __is_ranges_subrange_v = false;

template <class _Iter, class _Sent, ranges::subrange_kind _Kind>
inline constexpr bool __is_ranges_subrange_v<ranges::subrange<_Iter, _Sent, _Kind>> = true;

// template <class _Tp>
// concept __tuple_like = __tuple_like_no_subrange<_Tp> || __is_ranges_subrange_v<remove_cvref_t<_Tp>>;

template <class _Tp>
concept __has_tuple_size =
requires {
    typename std::tuple_size<_Tp>::type;
};

template <class _Tp, std::size_t _Nidx>
concept __has_tuple_element =
    requires(_Tp __t) {
        typename std::tuple_size<_Tp>::type;
        requires _Nidx < std::tuple_size_v<_Tp>;
        typename std::tuple_element_t<_Nidx, _Tp>;
        { std::get_element<_Nidx>(__t) } -> std::convertible_to<const std::tuple_element_t<_Nidx, _Tp>&>;
    };

// template <class _Tuple, std::size_t _TupleSize, std::size_t _Idx>
// struct __has_tuple_element_helper_impl {
//     static constexpr bool value = __has_tuple_element<_Tuple, _Idx> &&
//                                   __has_tuple_element_helper_impl<_Tuple, _TupleSize, _Idx + 1>::value;
// };

// template <class _Tuple, std::size_t _TupleSize>
// struct __has_tuple_element_helper_impl<_Tuple, _TupleSize, _TupleSize - 1> {
//     static constexpr bool value = __has_tuple_element<_Tuple, _TupleSize - 1>;
// };

// template <class _Tuple>
// struct __has_tuple_element_helper : __has_tuple_element_helper_impl<_Tuple, std::tuple_size_v<_Tuple>, 0> {};

template <class _Tp>
concept __tuple_like = !is_reference_v<_Tp> &&
                       __has_tuple_size<_Tp> &&
                       []<std::size_t... _Idx>(std::index_sequence<_Idx...>) {
                        //    return (... && __has_tuple_element<_Tp, _Idx>);
                        return std::conjunction_v<std::bool_constant<__has_tuple_element<_Tp, _Idx>>...>;
                       } (std::make_index_sequence<std::tuple_size_v<_Tp>>{});
                    //    __has_tuple_element_helper<_Tp>::value;

// As of writing this comment every use of `pair-like` in the standard excludes `ranges::subrange`, so
// you most likely want to use `__pair_like_no_subrange` if you're looking for `pair-like`.

#endif // _LIBCPP_STD_VER >= 20

_LIBCPP_END_NAMESPACE_STD

#endif // _LIBCPP___TUPLE_TUPLE_LIKE_H
