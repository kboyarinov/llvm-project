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

#include "test_comparisons.h"
#include "test_macros.h"

int main(int, char**) {
    {
        std::vector<LessCompOnly<0>> c;
        // expected-error@*:* {{invalid operands to binary expression}}
        TEST_IGNORE_NODISCARD (c == c);
    }
    {
        std::vector<LessCompOnly<1>> c;
        // expected-error@*:* {{invalid operands to binary expression}}
        TEST_IGNORE_NODISCARD (c != c);
    }
    {
        std::vector<EqCompOnly<0>> c;
        // expected-error@*:* {{invalid operands to binary expression}}
        TEST_IGNORE_NODISCARD (c < c);
    }
    {
        std::vector<EqCompOnly<1>> c;
        // expected-error@*:* {{invalid operands to binary expression}}
        TEST_IGNORE_NODISCARD (c <= c);
    }
    {
        std::vector<EqCompOnly<2>> c;
        // expected-error@*:* {{invalid operands to binary expression}}
        TEST_IGNORE_NODISCARD (c > c);
    }
    {
        std::vector<EqCompOnly<3>> c;
        // expected-error@*:* {{invalid operands to binary expression}}
        TEST_IGNORE_NODISCARD (c >= c);
    }
}
