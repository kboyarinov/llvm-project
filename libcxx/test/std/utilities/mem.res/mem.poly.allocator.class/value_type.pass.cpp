//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <memory_resource>

// template <class T> class polymorphic_allocator

// using value_type = T;

#include <memory_resource>

#include "test_macros.h"

int main(int, char**)
{
    {
        using alloc_type = std::pmr::polymorphic_allocator<int>;
        ASSERT_SAME_TYPE(alloc_type::value_type, int);
    }
#if TEST_STD_VER > 17
    {
        using alloc_type = std::pmr::polymorphic_allocator<>;
        ASSERT_SAME_TYPE(alloc_type::value_type, std::byte);
    }
#endif
    return 0;
}
