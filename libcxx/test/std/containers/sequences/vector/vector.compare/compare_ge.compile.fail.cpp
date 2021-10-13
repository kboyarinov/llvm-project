//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// <vector>

// bool operator>=(const vector& lhs, const vector& rhs);

#include <vector>
#include <cassert>

#include "compare_types.h"

int main(int, char**) {
    const std::vector<EqCompOnly> c1, c2;
    assert(c1 >= c2);
}
