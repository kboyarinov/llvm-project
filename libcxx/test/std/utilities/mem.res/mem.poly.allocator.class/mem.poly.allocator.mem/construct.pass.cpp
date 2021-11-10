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

// template <class U, class... Args>
// void polymorphic_allocator<T>::construct(U* p, Args&&... args)

#include <memory_resource>
#include <memory>
#include <type_traits>
#include <cassert>

#include "test_macros.h"
#include "uses_alloc_types.h"
#include "test_allocator.h"

template <class T, class Arg>
void testConstructBasic(Arg&& arg, UsesAllocatorType expected) {
    std::pmr::polymorphic_allocator<int> int_alloc;
    std::pmr::polymorphic_allocator<T> alloc = int_alloc;

    T* p = alloc.allocate(1);

    int_alloc.construct(p, std::forward<Arg>(arg));
    checkConstruct<Arg&&>(*p, expected);

    p->~T();
    alloc.deallocate(p, 1);
}

template <class T>
void testConstruct(UsesAllocatorType expected) {
    int arg = 1;
    const int& carg = arg;

    testConstructBasic<T>(arg, expected);
    testConstructBasic<T>(carg, expected);
    testConstructBasic<T>(std::move(arg), expected);
    testConstructBasic<T>(std::move(carg), expected);
}

template <class Alloc>
void testDoesNotUse() {
    using DoesNotUseType = NotUsesAllocator<Alloc, /*Args = */1>;
    testConstruct<DoesNotUseType>(UA_None);
}

template <class Alloc>
void testUsesLeading() {
    using UsesLeadingType = UsesAllocatorV1<Alloc, /*Args = */1>;
    testConstruct<UsesLeadingType>(UA_AllocArg);
}

template <class Alloc>
void testUsesTrailing() {
    using UsesTrailingType = UsesAllocatorV2<Alloc, /*Args = */1>;
    testConstruct<UsesTrailingType>(UA_AllocLast);
}

template <class Alloc>
void testUsesBoth() {
    using UsesBoth = UsesAllocatorV3<Alloc, /*Args = */1>;
    testConstruct<UsesBoth>(UA_AllocArg);
}

int main(int, char**)
{
    {
        std::pmr::polymorphic_allocator<int> alloc;
        int* ptr = nullptr;
        ASSERT_SAME_TYPE(decltype(alloc.construct(ptr, 1)), void);
        ASSERT_NOT_NOEXCEPT(alloc.construct(ptr, 1));
    }
    {
        testDoesNotUse<std::pmr::memory_resource*>();
        testDoesNotUse<std::pmr::polymorphic_allocator<int>>();
        testDoesNotUse<std::allocator<int>>();
        testDoesNotUse<test_allocator<int>>();
    }
    {
        testUsesLeading<std::pmr::memory_resource*>();
        testUsesLeading<std::pmr::polymorphic_allocator<int>>();
    }
    {
        testUsesTrailing<std::pmr::memory_resource*>();
        testUsesTrailing<std::pmr::polymorphic_allocator<int>>();
    }
    {
        testUsesBoth<std::pmr::memory_resource*>();
        testUsesBoth<std::pmr::polymorphic_allocator<int>>();
    }

    return 0;
}
