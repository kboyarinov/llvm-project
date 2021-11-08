//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14

// <memory_resource>

// void deallocate(void* ptr, size_t bytes, size_t alignment = max_align)

#include <memory_resource>
#include <cassert>

#include "test_macros.h"
#include "test_pmr.h"

int main(int, char**)
{
    TestResource r(42);
    auto& controller = r.getController();
    std::pmr::memory_resource* resource = &r;

    {
        ASSERT_SAME_TYPE(decltype(resource->deallocate(nullptr, 0, 0)), void);
        ASSERT_SAME_TYPE(decltype(resource->deallocate(nullptr, 0)), void);
    }
    {
        ASSERT_NOT_NOEXCEPT(resource->deallocate(nullptr, 0, 0));
        ASSERT_NOT_NOEXCEPT(resource->deallocate(nullptr, 0));
    }
    {
        std::size_t size = 100;
        std::size_t align = 64;
        void* p = resource->allocate(size, align);
        resource->deallocate(p, size, align);
        assert(controller.dealloc_count == 1);
        assert(controller.checkDealloc(p, size, align));
    }
    {
        std::size_t size = 128;
        std::size_t align = MaxAlignV;
        void* p = resource->allocate(size);
        resource->deallocate(p, size);
        assert(controller.dealloc_count == 2);
        assert(controller.checkDealloc(p, size, align));
    }

    return 0;
}
