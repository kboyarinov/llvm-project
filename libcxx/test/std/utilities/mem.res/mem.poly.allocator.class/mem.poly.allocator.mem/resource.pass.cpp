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

// memory_resource* polymorphic_allocator<T>::resource() const

#include <memory_resource>
#include <cassert>

#include "test_macros.h"
#include "test_pmr.h"

int main(int, char**)
{
    {
        const std::pmr::polymorphic_allocator<int> alloc;
        ASSERT_SAME_TYPE(decltype(alloc.resource()), std::pmr::memory_resource*);
        ASSERT_NOT_NOEXCEPT(alloc.resource());
    }
    {
        TestResource r;
        const std::pmr::polymorphic_allocator<int> alloc = &r;
        assert(alloc.resource() == &r);
    }
    {
        const std::pmr::polymorphic_allocator<int> alloc = nullptr;
        assert(alloc.resource() == nullptr);
    }
    {
        const std::pmr::polymorphic_allocator<int> alloc;
        assert(alloc.resource() == std::pmr::get_default_resource());
        assert(alloc.resource() == std::pmr::new_delete_resource());
    }
    {
        TestResource r;
        std::pmr::set_default_resource(&r);
        const std::pmr::polymorphic_allocator<int> alloc;
        assert(alloc.resource() == std::pmr::get_default_resource());
        assert(alloc.resource() == &r);
    }
    return 0;
}
