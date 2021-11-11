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

// template <class U, class... CtorArgs>
// [[nodiscard]] U* new_object(CtorArgs&&... ctor_args)

#include <memory_resource>
#include <cassert>

#include "test_macros.h"
#include "test_pmr.h"

template <std::size_t Size, std::size_t Align>
struct alignas(Align) Object {
    Object(int val) : value(val) { ++count; }

    ~Object() { --count; }

    static std::size_t count;
    int value;
    char array[Size];
};

template <std::size_t S, std::size_t A>
std::size_t Object<S, A>::count = 0;

template <class PolyAllocatorType, std::size_t Size, std::size_t Alignment>
void testDeleteObjectSizeAlignment() {
    using ObjectType = Object<Size, Alignment>;
    TestResource r(1);
    PolyAllocatorType alloc = &r;

    ObjectType* ret = alloc.template new_object<ObjectType>(1);
    assert(ObjectType::count == 1);

    alloc.delete_object(ret);
    assert(r.checkDealloc(ret, sizeof(ObjectType), alignof(ObjectType)));
    assert(ObjectType::count == 0);
    r.reset();
}

template <class PolyAllocatorType>
void testDeleteObjectForAlloc() {
    testDeleteObjectSizeAlignment<PolyAllocatorType, 1, 4>();
    testDeleteObjectSizeAlignment<PolyAllocatorType, 1, 8>();
    testDeleteObjectSizeAlignment<PolyAllocatorType, 1, alignof(std::max_align_t)>();
    testDeleteObjectSizeAlignment<PolyAllocatorType, 2, 4>();
    testDeleteObjectSizeAlignment<PolyAllocatorType, 2, alignof(std::max_align_t)>();
    testDeleteObjectSizeAlignment<PolyAllocatorType, 73, 4>();
    testDeleteObjectSizeAlignment<PolyAllocatorType, 73, alignof(std::max_align_t)>();
}

int main(int, char**)
{
    {
        std::pmr::polymorphic_allocator alloc;
        ASSERT_SAME_TYPE(decltype(alloc.delete_object<int>(nullptr)), void);
        ASSERT_NOT_NOEXCEPT(alloc.delete_object<int>(nullptr));
    }
    {
        testDeleteObjectForAlloc<std::pmr::polymorphic_allocator<>>();
        testDeleteObjectForAlloc<std::pmr::polymorphic_allocator<int>>();
    }

    return 0;
}
