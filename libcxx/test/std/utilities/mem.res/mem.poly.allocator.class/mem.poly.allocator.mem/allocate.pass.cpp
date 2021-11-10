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

// T* polymorphic_allocator<T>::allocate(size_t n)

#include <memory_resource>
#include <memory>
#include <cassert>
#include <limits>
#include <type_traits>

#include "test_macros.h"
#include "test_pmr.h"

template <std::size_t Size, std::size_t Alignment>
void testAllocateSizeAlignment() {
    using ValueType = std::aligned_storage_t<Size, Alignment>;
    TestResource r(1);
    std::pmr::polymorphic_allocator<ValueType> alloc = &r;

    for (std::size_t n = 0; n < 15; ++n) {
        ValueType* ret = alloc.allocate(n);
        assert(r.checkAlloc(ret, n * Size, Alignment));

        alloc.deallocate(ret, n);
        r.reset();
    }
}

#ifndef TEST_HAS_NO_EXCEPTIONS
template <std::size_t Size>
void testAllocateExceptionsForSize() {
    // using ValueType = std::aligned_storage_t<Size>;
    // using AllocType = std::pmr::polymorphic_allocator<ValueType>;
    // using Traits = std::allocator_traits<AllocType>;
    // TestResource r(1);

    // std::pmr::polymorphic_allocator<ValueType> alloc = &r;

    // std::size_t maxSize = Traits::max_size(alloc);
    // // try {
    // //     alloc.allocate(maxSize);
    // // } catch (...) {
    // //     assert(false);
    // // }

    // // try {
    // //     alloc.allocate(maxSize + 1);
    // //     assert(false);
    // // } catch (const std::bad_array_new_length&) {}
}
#endif

int main(int, char**)
{
    {
        std::pmr::polymorphic_allocator<int> alloc;
        ASSERT_SAME_TYPE(decltype(alloc.allocate(0)), int*);
        ASSERT_NOT_NOEXCEPT(alloc.allocate(0));
    }
    {
        testAllocateSizeAlignment<1, 1>();
        // testAllocateSizeAlignment<1, 2>();
        // testAllocateSizeAlignment<1, alignof(std::max_align_t)>();
        // testAllocateSizeAlignment<2, 2>();
        // testAllocateSizeAlignment<2, alignof(std::max_align_t)>();
        // testAllocateSizeAlignment<73, 2>();
        // testAllocateSizeAlignment<73, alignof(std::max_align_t)>();
    }
#ifndef TEST_HAS_NO_EXCEPTIONS
    {
        // testAllocateExceptionsForSize<1>();
        // testAllocateExceptionsForSize<2>();
        // testAllocateExceptionsForSize<4>();
        // testAllocateExceptionsForSize<16>();
        // testAllocateExceptionsForSize<73>();
    }
#endif
    return 0;
}
