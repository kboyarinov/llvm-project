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

// polymorphic_allocator<T>::polymorphic_allocator() noexcept

#include <memory_resource>
#include <cassert>

#include "test_macros.h"
#include "test_pmr.h"

int main(int, char**) {
    {
        {
            ASSERT_NOEXCEPT(std::pmr::polymorphic_allocator<int>());
        }
        {
            std::pmr::polymorphic_allocator<int> alloc;
            assert(alloc.resource() == std::pmr::get_default_resource());
            assert(alloc.resource() == std::pmr::new_delete_resource());
        }
        {
            TestResource r1;
            std::pmr::set_default_resource(&r1);

            std::pmr::polymorphic_allocator<int> alloc;
            assert(alloc.resource() == std::pmr::get_default_resource());
            assert(alloc.resource() == &r1);
        }
    }

    return 0;
}
