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
// [[nodiscard]] U* polymorphic_allocator<T>::allocate_object(size_t n = 1)

#include <memory_resource>
#include <memory>
#include <cassert>

#include "test_macros.h"
#include "test_pmr.h"

template <class PolyAllocatorType, std::size_t Size, std::size_t Alignment>
void testAllocateObjectSizeAlignment() {
    using ObjectType = std::aligned_storage_t<Size, Alignment>;
    TestResource r(1);
    PolyAllocatorType alloc = &r;

    for (std::size_t n = 1; n <= 10; ++n) {
        ObjectType* ret = nullptr;
        if (n == 1) {
            ret = alloc.template allocate_object<ObjectType>();
        } else {
            ret = alloc.template allocate_object<ObjectType>(n);
        }

        assert(ret != nullptr);
        assert(r.checkAlloc(ret, n * sizeof(ObjectType), alignof(ObjectType)));

        alloc.deallocate_object(ret, n);
        r.reset();
    }
}

template <class PolyAllocatorType>
void testAllocateObjectForAlloc() {
    testAllocateObjectSizeAlignment<PolyAllocatorType, 1, 1>();
    testAllocateObjectSizeAlignment<PolyAllocatorType, 1, 2>();
    testAllocateObjectSizeAlignment<PolyAllocatorType, 1, alignof(std::max_align_t)>();
    testAllocateObjectSizeAlignment<PolyAllocatorType, 2, 2>();
    testAllocateObjectSizeAlignment<PolyAllocatorType, 2, alignof(std::max_align_t)>();
    testAllocateObjectSizeAlignment<PolyAllocatorType, 73, 2>();
    testAllocateObjectSizeAlignment<PolyAllocatorType, 73, alignof(std::max_align_t)>();
}

#ifndef TEST_HAS_NO_EXCEPTIONS
template <class PolyAllocatorType, std::size_t Size>
void testAllocateObjectExceptionsForSize() {
    using ObjectType = std::aligned_storage_t<Size>;
    using ObjectAllocatorType = std::pmr::polymorphic_allocator<ObjectType>;
    using Traits = std::allocator_traits<ObjectAllocatorType>;
    TestResource r(1);

    PolyAllocatorType alloc = &r;
    std::size_t max_size = Traits::max_size(ObjectAllocatorType(alloc));
    try {
        [[maybe_unused]] ObjectType* ret = alloc.template
                                           allocate_object<ObjectType>(max_size + 1);
        assert(false);
    } catch (const std::bad_array_new_length&) {}
}

template <class PolyAllocatorType>
void testAllocateObjectExceptionsForAlloc() {
    testAllocateObjectExceptionsForSize<PolyAllocatorType, 2>();
    testAllocateObjectExceptionsForSize<PolyAllocatorType, 4>();
    testAllocateObjectExceptionsForSize<PolyAllocatorType, 16>();
    testAllocateObjectExceptionsForSize<PolyAllocatorType, 73>();
}
#endif

int main(int, char**)
{
    {
        std::pmr::polymorphic_allocator alloc;
        ASSERT_SAME_TYPE(decltype(alloc.allocate_object<int>()), int*);
        ASSERT_SAME_TYPE(decltype(alloc.allocate_object<int>(1)), int*);
    }
    {
        std::pmr::polymorphic_allocator alloc;
        ASSERT_NOT_NOEXCEPT(alloc.allocate_object<int>());
        ASSERT_NOT_NOEXCEPT(alloc.allocate_object<int>(1));
    }
    {
        testAllocateObjectForAlloc<std::pmr::polymorphic_allocator<>>();
        testAllocateObjectForAlloc<std::pmr::polymorphic_allocator<int>>();
    }
#ifndef TEST_HAS_NO_EXCEPTIONS
    {
        testAllocateObjectExceptionsForAlloc<std::pmr::polymorphic_allocator<>>();
        testAllocateObjectExceptionsForAlloc<std::pmr::polymorphic_allocator<int>>();
    }
#endif

    return 0;
}
