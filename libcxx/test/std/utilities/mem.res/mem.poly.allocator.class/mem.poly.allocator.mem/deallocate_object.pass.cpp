//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++03, c++11, c++14, c++17

// <memory_resource>

// template <class T> class polymorphic_allocator

// template <class U>
// void polymorphic_allocator<T>::deallocate_object(U* ptr, size_t n = 1)

#include <memory_resource>
#include <cassert>

#include "test_macros.h"
#include "test_pmr.h"

template <class PolyAllocatorType, std::size_t Size, std::size_t Alignment>
void testDeallocateObjectSizeAlignment() {
    using ObjectType = std::aligned_storage_t<Size, Alignment>;
    TestResource r(1);
    PolyAllocatorType alloc = &r;

    for (std::size_t n = 1; n <= 10; ++n) {
        ObjectType* ret = alloc.template allocate_object<ObjectType>(n);

        if (n == 1) {
            alloc.deallocate_object(ret);
        } else {
            alloc.deallocate_object(ret, n);
        }
        r.reset();
    }
}

template <class PolyAllocatorType>
void testDeallocateObjectForAlloc() {
    testDeallocateObjectSizeAlignment<PolyAllocatorType, 1, 1>();
    testDeallocateObjectSizeAlignment<PolyAllocatorType, 1, 2>();
    testDeallocateObjectSizeAlignment<PolyAllocatorType, 1, alignof(std::max_align_t)>();
    testDeallocateObjectSizeAlignment<PolyAllocatorType, 2, 2>();
    testDeallocateObjectSizeAlignment<PolyAllocatorType, 2, alignof(std::max_align_t)>();
    testDeallocateObjectSizeAlignment<PolyAllocatorType, 73, 2>();
    testDeallocateObjectSizeAlignment<PolyAllocatorType, 73, alignof(std::max_align_t)>();
}

int main(int, char**)
{
    {
        std::pmr::polymorphic_allocator alloc;
        ASSERT_SAME_TYPE(decltype(alloc.deallocate_object<int>(nullptr)), void);
        ASSERT_SAME_TYPE(decltype(alloc.deallocate_object<int>(nullptr, 1)), void);
    }
    {
        std::pmr::polymorphic_allocator alloc;
        ASSERT_NOT_NOEXCEPT(alloc.deallocate_object<int>(nullptr));
        ASSERT_NOT_NOEXCEPT(alloc.deallocate_object<int>(nullptr, 1));
    }
    {
        testDeallocateObjectForAlloc<std::pmr::polymorphic_allocator<>>();
        testDeallocateObjectForAlloc<std::pmr::polymorphic_allocator<int>>();
    }

    return 0;
}
