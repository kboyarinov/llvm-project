//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <memory_resource>

// memory_resource* null_memory_resource() noexcept;

#include <memory_resource>
#include <cassert>

#include "test_macros.h"
#include "count_new.h"
#include "test_pmr.h"

int main(int, char**)
{
    {
        ASSERT_SAME_TYPE(decltype(std::pmr::null_memory_resource()), std::pmr::memory_resource*);
    }
    {
        ASSERT_NOEXCEPT(std::pmr::null_memory_resource());
    }
    {
        assert(std::pmr::null_memory_resource() != nullptr);
    }
    {
        assert(std::pmr::null_memory_resource() == std::pmr::null_memory_resource());
    }
    {
        std::pmr::memory_resource* resource = std::pmr::null_memory_resource();
        DisableAllocationGuard g;
#ifndef TEST_HAS_NO_EXCEPTIONS
        try {
#endif
            [[maybe_unused]] void* result = resource->allocate(1);
#ifndef TEST_HAS_NO_EXCEPTIONS
            assert(false);
        } catch (const std::bad_alloc&) {}
#endif
    }
    {
        std::pmr::memory_resource* resource = std::pmr::null_memory_resource();
        DisableAllocationGuard g;
#ifndef TEST_HAS_NO_EXCEPTIONS
        try {
#endif
            [[maybe_unused]] void* result = resource->allocate(1, 4);
#ifndef TEST_HAS_NO_EXCEPTIONS
            assert(false);
        } catch (const std::bad_alloc&) {}
#endif
    }
    {
        globalMemCounter.reset();

        int x = 42;
        std::pmr::null_memory_resource()->deallocate(nullptr, 0);
        std::pmr::null_memory_resource()->deallocate(&x, 0);

        assert(globalMemCounter.checkDeleteCalledEq(0));
        assert(globalMemCounter.checkDeleteArrayCalledEq(0));

        std::pmr::null_memory_resource()->deallocate(nullptr, 0, 2);
        std::pmr::null_memory_resource()->deallocate(&x, 0, 2);

        assert(globalMemCounter.checkDeleteCalledEq(0));
        assert(globalMemCounter.checkDeleteArrayCalledEq(0));
    }
    {
        std::pmr::memory_resource* resource1 = std::pmr::null_memory_resource();
        std::pmr::memory_resource* resource2 = std::pmr::null_memory_resource();

        assert(*resource1 == *resource2);
        assert(*resource2 == *resource1);

        assert(!(*resource1 != *resource2));
        assert(!(*resource2 != *resource1));

        assert(resource1->is_equal(*resource2));
        assert(resource2->is_equal(*resource1));
    }
    {
        std::pmr::memory_resource* resource1 = std::pmr::null_memory_resource();
        TestResource r(1);
        std::pmr::memory_resource* resource2 = &r;

        assert(!(*resource1 == *resource2));
        assert(*resource1 != *resource2);

        assert(!resource1->is_equal(*resource2));
    }
    return 0;
}
