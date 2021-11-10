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

// polymorphic_allocator polymorphic_allocator<T>::select_on_container_copy_construction() const

#include <memory_resource>
#include <cassert>

#include "test_macros.h"
#include "test_pmr.h"

int main()
{
    {
        const std::pmr::polymorphic_allocator<int> alloc;
        ASSERT_SAME_TYPE(decltype(alloc.select_on_container_copy_construction()),
                         std::pmr::polymorphic_allocator<int>);
        ASSERT_NOT_NOEXCEPT(alloc.select_on_container_copy_construction());
    }
    {
        TestResource r(1);
        const std::pmr::polymorphic_allocator<int> alloc = &r;
        assert(alloc.resource() == &r);

        auto other = alloc.select_on_container_copy_construction();
        assert(other.resource() == std::pmr::get_default_resource());
        assert(alloc.resource() == &r);
    }
    {
        TestResource r(1);
        std::pmr::set_default_resource(&r);

        const std::pmr::polymorphic_allocator<int> alloc = std::pmr::new_delete_resource();
        assert(alloc.resource() == std::pmr::new_delete_resource());

        auto other = alloc.select_on_container_copy_construction();
        assert(other.resource() == std::pmr::get_default_resource());
        assert(other.resource() == &r);
        assert(alloc.resource() == std::pmr::new_delete_resource());
    }
    return 0;
}
