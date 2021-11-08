//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <memory_resource>

// bool operator==(const memory_resource& lhs,
//                 const memory_resource& rhs);

// bool operator!=(const memory_resource& lhs,
//                 const memory_resource& rhs);

#include <memory_resource>
#include <cassert>

#include "test_macros.h"
#include "test_pmr.h"
#include "test_comparisons.h"

int main(int, char**)
{
    {
        std::pmr::memory_resource const* resource = nullptr;
        ASSERT_SAME_TYPE(decltype(*resource == *resource), bool);
        ASSERT_SAME_TYPE(decltype(*resource != *resource), bool);
    }
    {
        std::pmr::memory_resource const* resource = nullptr;
        ASSERT_NOEXCEPT(*resource == *resource);
        ASSERT_NOEXCEPT(*resource != *resource);
    }
    {
        TestResource r1(1);
        std::pmr::memory_resource const* resource = &r1;

        assert(testComparisons2(*resource, *resource, true));
    }
    {
        TestResource r1(1);
        std::pmr::memory_resource const* resource1 = &r1;

        TestResource r2(1);
        std::pmr::memory_resource const* resource2 = &r2;

        assert(testComparisons2(*resource1, *resource2, true));
    }
    {
        TestResource r1(1);
        std::pmr::memory_resource const* resource1 = &r1;

        TestResource r2(2);
        std::pmr::memory_resource const* resource2 = &r2;

        assert(testComparisons2(*resource1, *resource2, false));
    }
    {
        TestResource1 r1(1);
        std::pmr::memory_resource const* resource1 = &r1;

        TestResource2 r2(1);
        std::pmr::memory_resource const* resource2 = &r2;

        assert(testComparisons2(*resource1, *resource2, false));
    }
    {
        CopyableResource r1(1), r2(r1);
        std::pmr::memory_resource const* resource1 = &r1;
        std::pmr::memory_resource const* resource2 = &r2;

        assert(testComparisons2(*resource1, *resource2, true));
    }
    {
        CopyableResource r1(1);
        std::pmr::memory_resource const* resource1 = &r1;

        CopyableResource r2(2);
        std::pmr::memory_resource const* resource2 = &r2;

        r2 = r1;

        assert(testComparisons2(*resource1, *resource2, true));
    }

    return 0;
}
