//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <memory_resource>

// memory_resource* new_delete_resource() noexcept;

#include <iostream>
#include <memory_resource>
#include <cassert>

#include "test_macros.h"
#include "count_new.h"
#include "test_pmr.h"

int main(int, char**)
{
    {
        ASSERT_SAME_TYPE(decltype(std::pmr::new_delete_resource()),
                         std::pmr::memory_resource*);
    }
    {
        ASSERT_NOEXCEPT(std::pmr::new_delete_resource());
    }
    {
        assert(std::pmr::new_delete_resource() != nullptr);
    }
    {
        assert(std::pmr::new_delete_resource() == std::pmr::new_delete_resource());
    }
    {
        std::pmr::memory_resource* resource = std::pmr::new_delete_resource();
        globalMemCounter.reset();

        void* ret = resource->allocate(50);
        assert(ret != nullptr);
        assert(globalMemCounter.checkNewCalledEq(1));
        assert(globalMemCounter.checkLastNewSizeEq(50));

        resource->deallocate(ret, 50);
        assert(globalMemCounter.checkDeleteCalledEq(1));
    }
    {
        std::pmr::memory_resource* resource = std::pmr::new_delete_resource();
        globalMemCounter.reset();
        std::size_t alignment = 16;

        void* ret = resource->allocate(50, alignment);
        assert(ret != nullptr);
        assert(globalMemCounter.checkAlignedNewCalledEq(1));
        assert(globalMemCounter.checkLastNewSizeEq(50));
        assert(globalMemCounter.checkLastNewAlignEq(alignment));

        resource->deallocate(ret, 50, alignment);
        assert(globalMemCounter.checkAlignedDeleteCalledEq(1));
        assert(globalMemCounter.checkLastDeleteAlignEq(alignment));
    }
    {
        std::pmr::memory_resource* resource1 = std::pmr::new_delete_resource();
        std::pmr::memory_resource* resource2 = std::pmr::new_delete_resource();

        assert(*resource1 == *resource2);
        assert(*resource2 == *resource1);

        assert(!(*resource1 != *resource2));
        assert(!(*resource2 != *resource1));

        assert(resource1->is_equal(*resource2));
        assert(resource2->is_equal(*resource1));
    }
    {
        std::pmr::memory_resource* resource1 = std::pmr::new_delete_resource();

        TestResource r(1);
        std::pmr::memory_resource* resource2 = &r;

        assert(!(*resource1 == *resource2));
        assert(*resource1 != *resource2);

        assert(!resource1->is_equal(*resource2));
    }
    return 0;
}
