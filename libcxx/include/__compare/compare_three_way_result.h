// -*- C++ -*-
//===-------------------------- compare -----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP___COMPARE_COMPARE_THREE_WAY_RESULT_H
#define _LIBCPP___COMPARE_COMPARE_THREE_WAY_RESULT_H

#include <__config>
#include <type_traits>

#if !defined(_LIBCPP_HAS_NO_PRAGMA_SYSTEM_HEADER)
#pragma GCC system_header
#endif

_LIBCPP_BEGIN_NAMESPACE_STD

#if _LIBCPP_STD_VER > 17 && !defined(_LIBCPP_HAS_NO_SPACESHIP_OPERATOR)

namespace __comp_detail {

// [cmp.result], result of three-way comparison
template <class _Tp, class _Up>
using __compare_three_way_result_type = decltype(declval<__make_const_lvalue_ref<_Tp>>() <=>
                                                 declval<__make_const_lvalue_ref<_Up>>());

} // namespace __comp_detail

template <class _Tp, class _Up = _Tp>
struct compare_three_way_result {};

template <class _Tp, class _Up>
    requires requires { typename __comp_detail::__compare_three_way_result_type<_Tp, _Up>; }
struct compare_three_way_result<_Tp, _Up> {
    using type = __comp_detail::__compare_three_way_result_type<_Tp, _Up>;
};

template <class _Tp, class _Up = _Tp>
using compare_three_way_result_t = typename compare_three_way_result<_Tp, _Up>::type;

#endif // _LIBCPP_STD_VER > 17 && !defined(_LIBCPP_HAS_NO_SPACESHIP_OPERATOR)

_LIBCPP_END_NAMESPACE_STD

#endif // _LIBCPP___COMPARE_COMPARE_THREE_WAY_RESULT_H
