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
#include "count_new.h"
#include "test_pmr.h"

#ifndef TEST_HAS_NO_EXCEPTIONS
struct ObjectConstructionException {};
#endif

template <std::size_t Size, std::size_t Align, bool ThrowOnConstruction = false>
struct alignas(Align) Object {
    Object(int val) : value(val) {
#ifndef TEST_HAS_NO_EXCEPTIONS
        if constexpr (ThrowOnConstruction) {
            throw ObjectConstructionException{};
        }
#endif
        ++count;
    }

    ~Object() { --count; }

    static std::size_t count;
    int value;
    char array[Size];
};

template <std::size_t S, std::size_t A, bool T>
std::size_t Object<S, A, T>::count = 0;

template <class PolyAllocatorType, std::size_t Size, std::size_t Alignment>
void testNewObjectSizeAlignment() {
    using ObjectType = Object<Size, Alignment>;
    TestResource r(1);
    PolyAllocatorType alloc = &r;

    ObjectType* ret = alloc.template new_object<ObjectType>(1);
    assert(ret != nullptr);
    assert(r.checkAlloc(ret, sizeof(ObjectType), alignof(ObjectType)));
    assert(ObjectType::count == 1);
    assert(ret->value == 1);

    alloc.delete_object(ret);
    r.reset();
}

template <class PolyAllocatorType>
void testNewObjectForAlloc() {
    testNewObjectSizeAlignment<PolyAllocatorType, 1, 4>();
    testNewObjectSizeAlignment<PolyAllocatorType, 1, 8>();
    testNewObjectSizeAlignment<PolyAllocatorType, 1, alignof(std::max_align_t)>();
    testNewObjectSizeAlignment<PolyAllocatorType, 2, 4>();
    testNewObjectSizeAlignment<PolyAllocatorType, 2, alignof(std::max_align_t)>();
    testNewObjectSizeAlignment<PolyAllocatorType, 73, 4>();
    testNewObjectSizeAlignment<PolyAllocatorType, 73, alignof(std::max_align_t)>();
}

#ifndef TEST_HAS_NO_EXCEPTIONS
template <class PolyAllocatorType, std::size_t Size>
void testNewObjectExceptionsForSize() {
    using ObjectType = Object<Size, alignof(std::max_align_t), /*Throw on ctor = */true>;
    globalMemCounter.reset();
    PolyAllocatorType alloc = std::pmr::new_delete_resource();

    try {
        [[maybe_unused]] ObjectType* ret = alloc.template new_object<ObjectType>(1);
        assert(false);
    } catch (const ObjectConstructionException&) {
        globalMemCounter.checkNewCalledEq(1);
        globalMemCounter.checkLastNewSizeEq(sizeof(ObjectType));
        globalMemCounter.checkDeleteCalledEq(1);
    }
}

template <class PolyAllocatorType>
void testNewObjectExceptionsForSize() {
    testNewObjectExceptionsForSize<PolyAllocatorType, 2>();
    testNewObjectExceptionsForSize<PolyAllocatorType, 4>();
    testNewObjectExceptionsForSize<PolyAllocatorType, 16>();
    testNewObjectExceptionsForSize<PolyAllocatorType, 73>();
}
#endif

int main(int, char**)
{
    {
        std::pmr::polymorphic_allocator alloc;
        ASSERT_SAME_TYPE(decltype(alloc.new_object<int>(1)), int*);
        ASSERT_NOT_NOEXCEPT(alloc.new_object<int>(1));
    }
    {
        testNewObjectForAlloc<std::pmr::polymorphic_allocator<>>();
        testNewObjectForAlloc<std::pmr::polymorphic_allocator<int>>();
    }
#ifndef TEST_HAS_NO_EXCEPTIONS
    {
        testNewObjectExceptionsForSize<std::pmr::polymorphic_allocator<>>();
        testNewObjectExceptionsForSize<std::pmr::polymorphic_allocator<int>>();
    }
#endif
    return 0;
}
