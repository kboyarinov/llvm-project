//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <memory_resource>

// template <class T, class U>
// bool operator==(const std::pmr::polymorphic_allocator<T>& lhs,
//                 const std::pmr::polymorphic_allocator<U>& rhs) noexcept;

// template <class T, class U>
// bool operator!=(const std::pmr::polymorphic_allocator<T>& lhs,
//                 const std::pmr::polymorphic_allocator<U>& rhs) noexcept;

#include <memory_resource>
#include <cassert>

#include "test_macros.h"
#include "test_pmr.h"
#include "test_comparisons.h"

int main(int, char**)
{
    {
        std::pmr::polymorphic_allocator<int> alloc;
        ASSERT_SAME_TYPE(decltype(alloc == alloc), bool);
        ASSERT_SAME_TYPE(decltype(alloc != alloc), bool);
    }
    {
        std::pmr::polymorphic_allocator<int> alloc;
        ASSERT_NOEXCEPT(alloc == alloc);
        ASSERT_NOEXCEPT(alloc != alloc);
    }
    {
        TestResource resource1(1);
        TestResource resource2(1);

        const std::pmr::polymorphic_allocator<int> alloc1 = &resource1;
        const std::pmr::polymorphic_allocator<int> alloc2 = &resource2;

        assert(testComparisons2(alloc1, alloc2, true));
    }
    {
        TestResource resource;
        const std::pmr::polymorphic_allocator<int> alloc1 = &resource;
        const std::pmr::polymorphic_allocator<int> alloc2 = &resource;

        assert(testComparisons2(alloc1, alloc2, true));
    }
    {
        TestResource resource1(1);
        TestResource resource2(2);

        const std::pmr::polymorphic_allocator<int> alloc1 = &resource1;
        const std::pmr::polymorphic_allocator<int> alloc2 = &resource2;

        assert(testComparisons2(alloc1, alloc2, false));
    }
    {
        TestResource resource1(1);
        TestResource resource2(1);

        const std::pmr::polymorphic_allocator<int> alloc1 = &resource1;
        const std::pmr::polymorphic_allocator<char> alloc2 = &resource2;

        assert(testComparisons2(alloc1, alloc2, true));
    }
    {
        TestResource resource;
        const std::pmr::polymorphic_allocator<int> alloc1 = &resource;
        const std::pmr::polymorphic_allocator<char> alloc2 = &resource;

        assert(testComparisons2(alloc1, alloc2, true));
    }
    {
        TestResource resource1(1);
        TestResource resource2(2);

        const std::pmr::polymorphic_allocator<int> alloc1 = &resource1;
        const std::pmr::polymorphic_allocator<char> alloc2 = &resource2;

        assert(testComparisons2(alloc1, alloc2, false));
    }

    return 0;
}
