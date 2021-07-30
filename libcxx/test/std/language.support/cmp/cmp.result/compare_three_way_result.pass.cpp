//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++98, c++03, c++11, c++14, c++17
// UNSUPPORTED: libcpp-no-concepts

// <compare>

// compare_three_way_result, compare_three_way_result_t

#include <compare>
#include <type_traits>

#include "test_macros.h"

template <class T, class U>
concept compare_three_way_result_type_defined = requires {
    typename std::compare_three_way_result<T, U>::type;
};

template <class T, class U>
concept compare_three_way_result_t_defined = requires {
    typename std::compare_three_way_result_t<T, U>;
};

template <class DesiredOrder, typename T, typename U = T>
constexpr void test_basic() {
    if constexpr (std::is_same_v<T, U>) {
        ASSERT_SAME_TYPE(std::compare_three_way_result_t<T>, DesiredOrder);
        ASSERT_SAME_TYPE(std::compare_three_way_result_t<T>,
                         typename std::compare_three_way_result<T>::type);
    }

    ASSERT_SAME_TYPE(std::compare_three_way_result_t<T, U>, DesiredOrder);
    ASSERT_SAME_TYPE(std::compare_three_way_result_t<U, T>, DesiredOrder);
    ASSERT_SAME_TYPE(std::compare_three_way_result_t<T, U>,
                     typename std::compare_three_way_result<T, U>::type);
}

constexpr void test_fundamentals() {
    test_basic<std::strong_ordering, int>();
    test_basic<std::strong_ordering, short int>();
    test_basic<std::strong_ordering, unsigned short int>();
    test_basic<std::strong_ordering, long int>();
    test_basic<std::strong_ordering, unsigned long int>();
    test_basic<std::strong_ordering, long long int>();
    test_basic<std::strong_ordering, unsigned long long int>();
    test_basic<std::strong_ordering, bool>();
    test_basic<std::strong_ordering, void*>();
    test_basic<std::strong_ordering, char*>();
    test_basic<std::strong_ordering, char const*>();
    test_basic<std::strong_ordering, char volatile*>();
    test_basic<std::strong_ordering, char const volatile*>();
    test_basic<std::strong_ordering, char>();
    test_basic<std::strong_ordering, signed char>();
    test_basic<std::strong_ordering, unsigned char>();
    test_basic<std::strong_ordering, wchar_t>();
#ifndef _LIBCPP_HAS_NO_CHAR8_T
    test_basic<std::strong_ordering, char8_t>();
#endif
#ifndef _LIBCPP_HAS_NO_UNICODE_CHARS
    test_basic<std::strong_ordering, char16_t>();
    test_basic<std::strong_ordering, char32_t>();
#endif
#ifndef _LIBCPP_HAS_NO_INT128
    test_basic<std::strong_ordering, __int128_t>();
    test_basic<std::strong_ordering, __uint128_t>();
#endif
    test_basic<std::strong_ordering, std::size_t>();

    test_basic<std::partial_ordering, float>();
    test_basic<std::partial_ordering, double>();
    test_basic<std::partial_ordering, long double>();
    test_basic<std::partial_ordering, int, float>();
    test_basic<std::partial_ordering, int, double>();
    test_basic<std::partial_ordering, int, long double>();

    static_assert(!compare_three_way_result_type_defined<void, void>);
    static_assert(!compare_three_way_result_t_defined<void, void>);
    static_assert(!compare_three_way_result_type_defined<std::nullptr_t, std::nullptr_t>);
    static_assert(!compare_three_way_result_t_defined<std::nullptr_t, std::nullptr_t>);
}

struct OtherOperand {
    friend std::strong_ordering operator<=>(OtherOperand, OtherOperand) = delete;
};

template <class SameOrder, class DifferentOrder>
struct Operand {
    SameOrder operator<=>(const Operand&) const = default;
    friend DifferentOrder operator<=>(Operand, OtherOperand) {
        return DifferentOrder::equivalent;
    }
};

template <class SameOrder, class DifferentOrder>
constexpr void test() {
    using OT = Operand<SameOrder, DifferentOrder>;
    test_basic<SameOrder, OT>();
    test_basic<SameOrder, OT&>();
    test_basic<DifferentOrder, OT, OtherOperand>();
    test_basic<DifferentOrder, OT&, OtherOperand&>();
}

constexpr void negative_test() {
    static_assert(!compare_three_way_result_type_defined<OtherOperand, OtherOperand>);
    static_assert(!compare_three_way_result_type_defined<OtherOperand, int>);
    static_assert(!compare_three_way_result_type_defined<int, OtherOperand>);

    static_assert(!compare_three_way_result_t_defined<OtherOperand, OtherOperand>);
    static_assert(!compare_three_way_result_t_defined<OtherOperand, int>);
    static_assert(!compare_three_way_result_t_defined<int, OtherOperand>);
}

int main(int, char**) {
    test_fundamentals();
    test<std::strong_ordering, std::weak_ordering>();
    test<std::weak_ordering, std::partial_ordering>();
    test<std::partial_ordering, std::strong_ordering>();
    negative_test();
}
