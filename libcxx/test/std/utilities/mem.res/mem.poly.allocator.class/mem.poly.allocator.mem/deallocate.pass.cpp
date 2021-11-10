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

// void polymorphic_allocator<T>::deallocate(T* p, size_t n)

#include <memory_resource>
#include <cassert>
#include <type_traits>

#include "test_macros.h"
#include "test_pmr.h"

template <std::size_t Size, std::size_t Alignment>
void testDeallocateSizeAlignment() {
    using ValueType = std::aligned_storage_t<Size, Alignment>;

    TestResource r(1);
    std::pmr::polymorphic_allocator<ValueType> alloc = &r;

    for (std::size_t n = 0; n < 10; ++n) {
        ValueType* ret = alloc.allocate(n);

        alloc.deallocate(ret, n);
        assert(r.checkDealloc(ret, n * Size, Alignment));

        r.reset();
    }
}

int main(int, char**)
{
    {
        std::pmr::polymorphic_allocator<int> alloc;
        ASSERT_SAME_TYPE(decltype(alloc.deallocate(nullptr, 0)), void);
        ASSERT_NOT_NOEXCEPT(alloc.deallocate(nullptr, 0));
    }
    {
        testDeallocateSizeAlignment<1, 1>();
        testDeallocateSizeAlignment<1, 2>();
        testDeallocateSizeAlignment<1, alignof(std::max_align_t)>();
        testDeallocateSizeAlignment<2, 2>();
        testDeallocateSizeAlignment<2, alignof(std::max_align_t)>();
        testDeallocateSizeAlignment<73, 2>();
        testDeallocateSizeAlignment<73, alignof(std::max_align_t)>();
    }

    return 0;
}
