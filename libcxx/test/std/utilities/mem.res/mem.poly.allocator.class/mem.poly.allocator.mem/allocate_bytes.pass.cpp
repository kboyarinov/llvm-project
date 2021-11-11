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

// [[nodiscard]] void* polymorphic_allocator<T>::allocate_bytes(size_t nbytes,
//                                                              size_t alignment = alignof(std::max_align_t))

#include <memory_resource>
#include <cassert>

#include "test_macros.h"
#include "test_pmr.h"

template <class PolyAllocatorType, std::size_t Size, std::size_t Alignment>
void testAllocateBytesSizeAlignment() {
    TestResource r(1);
    PolyAllocatorType alloc = &r;

    for (std::size_t n = 1; n <= 10; ++n) {
        void* ret = nullptr;
        if constexpr (Alignment == alignof(std::max_align_t)) {
            ret = alloc.allocate_bytes(n * Size);
        } else {
            ret = alloc.allocate_bytes(n * Size, Alignment);
        }
        assert(ret != nullptr);
        assert(r.checkAlloc(ret, n * Size, Alignment));

        alloc.deallocate_bytes(ret, n * Size, Alignment);
        r.reset();
    }
}

template <class PolyAllocatorType>
void testAllocateBytesForAlloc() {
    testAllocateBytesSizeAlignment<PolyAllocatorType, 1, 1>();
    testAllocateBytesSizeAlignment<PolyAllocatorType, 1, 2>();
    testAllocateBytesSizeAlignment<PolyAllocatorType, 1, alignof(std::max_align_t)>();
    testAllocateBytesSizeAlignment<PolyAllocatorType, 2, 2>();
    testAllocateBytesSizeAlignment<PolyAllocatorType, 2, alignof(std::max_align_t)>();
    testAllocateBytesSizeAlignment<PolyAllocatorType, 73, 2>();
    testAllocateBytesSizeAlignment<PolyAllocatorType, 73, alignof(std::max_align_t)>();
}

int main(int, char**)
{
    {
        std::pmr::polymorphic_allocator alloc;
        ASSERT_SAME_TYPE(decltype(alloc.allocate_bytes(0)), void*);
        ASSERT_SAME_TYPE(decltype(alloc.allocate_bytes(0, 0)), void*);
    }
    {
        std::pmr::polymorphic_allocator alloc;
        ASSERT_NOT_NOEXCEPT(alloc.allocate_bytes(0));
        ASSERT_NOT_NOEXCEPT(alloc.allocate_bytes(0, 0));
    }
    {
        testAllocateBytesForAlloc<std::pmr::polymorphic_allocator<>>();
        testAllocateBytesForAlloc<std::pmr::polymorphic_allocator<int>>();
    }

    return 0;
}
