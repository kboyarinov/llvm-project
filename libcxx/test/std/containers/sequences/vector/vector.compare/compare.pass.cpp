//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <vector>

// bool operator==(const vector& lhs, const vector& rhs);
// bool operator!=(const vector& lhs, const vector& rhs);
// bool operator< (const vector& lhs, const vector& rhs);
// bool operator<=(const vector& lhs, const vector& rhs);
// bool operator> (const vector& lhs, const vector& rhs);
// bool operator>=(const vector& lhs, const vector& rhs);

#include <vector>
#include <cassert>
#include <cstdint>

#include "compare_types.h"

int main(int, char**) {
    {
        const std::vector<std::int32_t> c1, c2;

        test_equality_comparisons</*ExpectEqual = */true>(c1, c2);
        test_order_comparisons</*ExpectLess = */false,
                               /*ExpectEqual = */true>(c1, c2);
    }
    {
        const std::vector<std::int32_t> c1{1}, c2{2};

        test_equality_comparisons</*ExpectEqual = */false>(c1, c2);
        test_order_comparisons</*ExpectLess = */true,
                               /*ExpectEqual = */false>(c1, c2);
    }
    {
        const std::vector<std::int32_t> c1, c2{2};

        test_equality_comparisons</*ExpectEqual = */false>(c1, c2);
        test_order_comparisons</*ExpectLess = */true,
                               /*ExpectEqual = */false>(c1, c2);
    }
    {
        const std::vector<std::int32_t> c1{1, 2, 1}, c2{1, 2, 2};

        test_equality_comparisons</*ExpectEqual = */false>(c1, c2);
        test_order_comparisons</*ExpectLess = */true,
                               /*ExpectEqual = */false>(c1, c2);
    }
    {
        const std::vector<std::int32_t> c1{3, 2, 3}, c2{3, 1, 3};

        test_equality_comparisons</*ExpectEqual = */false>(c1, c2);
        test_order_comparisons</*ExpectLess = */false,
                               /*ExpectEqual = */false>(c1, c2);
    }
    {
        const std::vector<std::int32_t> c1{1, 2}, c2{1, 2, 0};

        test_equality_comparisons</*ExpectEqual = */false>(c1, c2);
        test_order_comparisons</*ExpectLess = */true,
                               /*ExpectEqual = */false>(c1, c2);
    }
    {
        const std::vector<std::int32_t> c1{1, 2, 0}, c2{3};

        test_equality_comparisons</*ExpectEqual = */false>(c1, c2);
        test_order_comparisons</*ExpectLess = */true,
                               /*ExpectEqual = */false>(c1, c2);
    }
    {
        const std::vector<LessAndEqComp> c1, c2;

        test_equality_comparisons</*ExpectEqual = */true>(c1, c2);
        test_order_comparisons</*ExpectLess = */false,
                               /*ExpectEqual = */true>(c1, c2);
    }
    {
        const std::vector<LessAndEqComp> c1{{1, 2}}, c2{{1, 2}};

        test_equality_comparisons</*ExpectEqual = */true>(c1, c2);
        test_order_comparisons</*ExpectLess = */false,
                               /*ExpectEqual = */true>(c1, c2);
    }
    {
        const std::vector<LessAndEqComp> c1{{1, 1}}, c2{{1, 2}};

        test_equality_comparisons</*ExpectEqual = */false>(c1, c2);
        test_order_comparisons</*ExpectLess = */true,
                               /*ExpectEqual = */false>(c1, c2);
    }
    {
        const std::vector<LessAndEqComp> c1, c2{{2, 2}};

        test_equality_comparisons</*ExpectEqual = */false>(c1, c2);
        test_order_comparisons</*ExpectLess = */true,
                               /*ExpectEqual = */false>(c1, c2);
    }
    {
        const std::vector<LessAndEqComp> c1{{1, 1}, {2, 2}, {2, 0}};
        const std::vector<LessAndEqComp> c2{{1, 1}, {2, 2}, {1, 0}};

        test_equality_comparisons</*ExpectEqual = */false>(c1, c2);
        test_order_comparisons</*ExpectLess = */false,
                               /*ExpectEqual = */false>(c1, c2);
    }
    {
        const std::vector<LessAndEqComp> c1{{3, 3}, {3, 3}, {3, 3}};
        const std::vector<LessAndEqComp> c2{{3, 3}, {2, 2}, {3, 3}};

        test_equality_comparisons</*ExpectEqual = */false>(c1, c2);
        test_order_comparisons</*ExpectLess = */false,
                               /*ExpectEqual = */false>(c1, c2);
    }
    {
        const std::vector<LessAndEqComp> c1{{1, 1}, {2, 2}};
        const std::vector<LessAndEqComp> c2{{1, 1}, {2, 2}, {0, 0}};

        test_equality_comparisons</*ExpectEqual = */false>(c1, c2);
        test_order_comparisons</*ExpectLess = */true,
                               /*ExpectEqual = */false>(c1, c2);
    }
    {
        const std::vector<LessAndEqComp> c1 = {{1, 1}, {2, 2}, {0, 0}};
        const std::vector<LessAndEqComp> c2 = {{3, 3}};

        test_equality_comparisons</*ExpectEqual = */false>(c1, c2);
        test_order_comparisons</*ExpectLess = */false,
                               /*ExpectEqual = */false>(c1, c2);
    }
    {
        assert((std::vector<std::int32_t>() == std::vector<std::int32_t>()));
        assert(!(std::vector<std::int32_t>() != std::vector<std::int32_t>()));
        assert(!(std::vector<std::int32_t>() < std::vector<std::int32_t>()));
        assert((std::vector<std::int32_t>() <= std::vector<std::int32_t>()));
        assert(!(std::vector<std::int32_t>() > std::vector<std::int32_t>()));
        assert((std::vector<std::int32_t>() >= std::vector<std::int32_t>()));
    }
}
