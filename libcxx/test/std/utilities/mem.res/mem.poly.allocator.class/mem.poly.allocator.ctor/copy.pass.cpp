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

// polymorphic_allocator<T>::polymorphic_allocator(const polymorphic_allocator& other)

#include <memory_resource>
#include <cassert>

#include "test_macros.h"
#include "test_pmr.h"

int main(int, char**)
{
    {
        const std::pmr::polymorphic_allocator<int> alloc1;
        std::pmr::polymorphic_allocator<int> alloc2 = alloc1;
        assert(alloc1.resource() == alloc2.resource());
        assert(alloc2.resource() == std::pmr::get_default_resource());
        assert(alloc2.resource() == std::pmr::new_delete_resource());
    }
    {
        TestResource r(1);
        const std::pmr::polymorphic_allocator<int> alloc1(&r);
        std::pmr::polymorphic_allocator<int> alloc2 = alloc1;
        assert(alloc2.resource() == alloc1.resource());
        assert(alloc2.resource() == &r);
    }

    return 0;
}
