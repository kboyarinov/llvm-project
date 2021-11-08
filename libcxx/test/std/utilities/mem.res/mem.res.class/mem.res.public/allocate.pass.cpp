//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <memory_resource>

// void* allocate(size_t bytes, size_t alignment = max_align)

#include <memory_resource>
#include <cassert>
#include <cstdint>

#include "test_macros.h"
#include "test_pmr.h"

int main(int, char**)
{
    TestResource r(42);
    auto& controller = r.getController();
    std::pmr::memory_resource* resource = &r;

    {
        ASSERT_SAME_TYPE(decltype(resource->allocate(0, 0)), void*);
        ASSERT_SAME_TYPE(decltype(resource->allocate(0)), void*);
    }
    {
        ASSERT_NOT_NOEXCEPT(resource->allocate(0, 0));
        ASSERT_NOT_NOEXCEPT(resource->allocate(0));
    }
    {
        std::size_t size = 42;
        std::size_t align = 64;
        void* p = resource->allocate(size, align);
        assert(controller.alloc_count == 1);
        assert(controller.checkAlloc(p, size, align));
        resource->deallocate(p, size, align);
    }
    {
        std::size_t size = 128;
        std::size_t align = MaxAlignV;
        void* p = resource->allocate(size);
        assert(controller.alloc_count == 2);
        assert(controller.checkAlloc(p, size, align));
        resource->deallocate(p, size);
    }
#ifndef TEST_HAS_NO_EXCEPTIONS
    {
        controller.throw_on_alloc = true;
        try {
            [[maybe_unused]] void* ptr = resource->allocate(42);
            assert(false);
        } catch (TestException) {}
        controller.throw_on_alloc = false;
    }
#endif

    return 0;
}
