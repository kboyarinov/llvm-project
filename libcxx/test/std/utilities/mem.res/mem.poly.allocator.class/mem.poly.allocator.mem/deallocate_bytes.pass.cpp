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

// void polymorphic_allocator<T>::deallocate_bytes(void* ptr, size_t nbytes,
//                                                 size_t alignment = alignof(std::max_align_t))

#include <memory_resource>
#include <cassert>

#include "test_macros.h"
#include "test_pmr.h"

template <class PolyAllocatorType, std::size_t Size, std::size_t Alignment>
void testDeallocateBytesSizeAlignment() {
    TestResource r(1);
    PolyAllocatorType alloc = &r;

    for (std::size_t n = 1; n <= 10; ++n) {
        void* ret = alloc.allocate_bytes(n * Size, Alignment);

        if constexpr (Alignment == alignof(std::max_align_t)) {
            alloc.deallocate_bytes(ret, n * Size);
        } else {
            alloc.deallocate_bytes(ret, n * Size, Alignment);
        }
        assert(r.checkDealloc(ret, n * Size, Alignment));
        r.reset();
    }
}

template <class PolyAllocatorType>
void testDeallocateBytesForAlloc() {
    testDeallocateBytesSizeAlignment<PolyAllocatorType, 1, 1>();
    testDeallocateBytesSizeAlignment<PolyAllocatorType, 1, 2>();
    testDeallocateBytesSizeAlignment<PolyAllocatorType, 1, alignof(std::max_align_t)>();
    testDeallocateBytesSizeAlignment<PolyAllocatorType, 2, 2>();
    testDeallocateBytesSizeAlignment<PolyAllocatorType, 2, alignof(std::max_align_t)>();
    testDeallocateBytesSizeAlignment<PolyAllocatorType, 73, 2>();
    testDeallocateBytesSizeAlignment<PolyAllocatorType, 73, alignof(std::max_align_t)>();
}

int main(int, char**)
{
    {
        std::pmr::polymorphic_allocator alloc;
        ASSERT_SAME_TYPE(decltype(alloc.deallocate_bytes(nullptr, 0)), void);
        ASSERT_SAME_TYPE(decltype(alloc.deallocate_bytes(nullptr, 0, 0)), void);
    }
    {
        std::pmr::polymorphic_allocator alloc;
        ASSERT_NOT_NOEXCEPT(alloc.deallocate_bytes(nullptr, 0));
        ASSERT_NOT_NOEXCEPT(alloc.deallocate_bytes(nullptr, 0, 0));
    }
    {
        testDeallocateBytesForAlloc<std::pmr::polymorphic_allocator<>>();
        testDeallocateBytesForAlloc<std::pmr::polymorphic_allocator<int>>();
    }

    return 0;
}
