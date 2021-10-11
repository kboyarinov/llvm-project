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

        assert((c1 == c2));
        assert(!(c1 != c2));
        assert(!(c1 < c2));
        assert((c1 <= c2));
        assert(!(c1 > c2));
        assert((c1 >= c2));
    }
    {
        const std::vector<std::int32_t> c1{1}, c2{2};

        assert(!(c1 == c2));
        assert((c1 != c2));
        assert((c1 < c2));
        assert((c1 <= c2));
        assert((!(c1 > c2)));
        assert((!(c1 >= c2)));
    }
    {
        const std::vector<std::int32_t> c1{2}, c2{1};

        assert(!(c1 == c2));
        assert((c1 != c2));
        assert(!(c1 < c2));
        assert(!(c1 <= c2));
        assert((c1 > c2));
        assert((c1 >= c2));
    }
    {
        const std::vector<std::int32_t> c1, c2{2};

        assert(!(c1 == c2));
        assert((c1 != c2));
        assert((c1 < c2));
        assert((c1 <= c2));
        assert(!(c1 > c2));
        assert(!(c1 >= c2));
    }
    {
        const std::vector<std::int32_t> c1{1}, c2;

        assert(!(c1 == c2));
        assert((c1 != c2));
        assert(!(c1 < c2));
        assert(!(c1 <= c2));
        assert((c1 > c2));
        assert((c1 >= c2));
    }
    {
        const std::vector<std::int32_t> c1{1, 2, 1}, c2{1, 2, 2};

        assert(!(c1 == c2));
        assert((c1 != c2));
        assert((c1 < c2));
        assert((c1 <= c2));
        assert(!(c1 > c2));
        assert(!(c1 >= c2));
    }
    {
        const std::vector<std::int32_t> c1{1, 2, 2}, c2{1, 2, 1};

        assert(!(c1 == c2));
        assert((c1 != c2));
        assert(!(c1 < c2));
        assert(!(c1 <= c2));
        assert((c1 > c2));
        assert((c1 >= c2));
    }
    {
        const std::vector<std::int32_t> c1{3, 2, 3}, c2{3, 1, 3};

        assert(!(c1 == c2));
        assert((c1 != c2));
        assert(!(c1 < c2));
        assert(!(c1 <= c2));
        assert((c1 > c2));
        assert((c1 >= c2));
    }
    {
        const std::vector<std::int32_t> c1{1, 2}, c2{1, 2, 0};

        assert(!(c1 == c2));
        assert((c1 != c2));
        assert((c1 < c2));
        assert((c1 <= c2));
        assert(!(c1 > c2));
        assert(!(c1 >= c2));
    }
    {
        const std::vector<std::int32_t> c1{1, 2, 0}, c2{1, 2};

        assert(!(c1 == c2));
        assert((c1 != c2));
        assert(!(c1 < c2));
        assert(!(c1 <= c2));
        assert((c1 > c2));
        assert((c1 >= c2));
    }
    {
        const std::vector<std::int32_t> c1{1, 2, 0}, c2{3};

        assert(!(c1 == c2));
        assert((c1 != c2));
        assert((c1 < c2));
        assert((c1 <= c2));
        assert(!(c1 > c2));
        assert(!(c1 >= c2));
    }
    {
        const std::vector<std::int32_t> c1{3}, c2{1, 2, 0};

        assert(!(c1 == c2));
        assert((c1 != c2));
        assert(!(c1 < c2));
        assert(!(c1 <= c2));
        assert((c1 > c2));
        assert((c1 >= c2));
    }
    {
        const std::vector<LessAndEqComp> c1, c2;

        assert((c1 == c2));
        assert(!(c1 != c2));
        assert(!(c1 < c2));
        assert((c1 <= c2));
        assert(!(c1 > c2));
        assert((c1 >= c2));
    }
    {
        const std::vector<LessAndEqComp> c1{{1, 2}}, c2{{1, 2}};

        assert((c1 == c2));
        assert(!(c1 != c2));
        assert(!(c1 < c2));
        assert((c1 <= c2));
        assert(!(c1 > c2));
        assert((c1 >= c2));
    }
    {
        const std::vector<LessAndEqComp> c1{{1, 1}}, c2{{1, 2}};

        assert(!(c1 == c2));
        assert((c1 != c2));
        assert((c1 < c2));
        assert((c1 <= c2));
        assert(!(c1 > c2));
        assert(!(c1 >= c2));
    }
    {
        const std::vector<LessAndEqComp> c1{{1, 1}}, c2{{1, 2}};

        assert(!(c1 == c2));
        assert((c1 != c2));
        assert((c1 < c2));
        assert((c1 <= c2));
        assert(!(c1 > c2));
        assert(!(c1 >= c2));
    }
    {
        const std::vector<LessAndEqComp> c1{{1, 2}}, c2{{1, 1}};

        assert(!(c1 == c2));
        assert((c1 != c2));
        assert(!(c1 < c2));
        assert(!(c1 <= c2));
        assert((c1 > c2));
        assert((c1 >= c2));
    }
    {
        const std::vector<LessAndEqComp> c1, c2{{2, 2}};

        assert(!(c1 == c2));
        assert((c1 != c2));
        assert((c1 < c2));
        assert((c1 <= c2));
        assert(!(c1 > c2));
        assert(!(c1 >= c2));
    }
    {
        const std::vector<LessAndEqComp> c1{{2, 2}}, c2;

        assert(!(c1 == c2));
        assert((c1 != c2));
        assert(!(c1 < c2));
        assert(!(c1 <= c2));
        assert((c1 > c2));
        assert((c1 >= c2));
    }
    {
        const std::vector<LessAndEqComp> c1{{1, 1}, {2, 2}, {2, 0}};
        const std::vector<LessAndEqComp> c2{{1, 1}, {2, 2}, {1, 0}};

        assert(!(c1 == c2));
        assert((c1 != c2));
        assert(!(c1 < c2));
        assert(!(c1 <= c2));
        assert((c1 > c2));
        assert((c1 >= c2));
    }
}
