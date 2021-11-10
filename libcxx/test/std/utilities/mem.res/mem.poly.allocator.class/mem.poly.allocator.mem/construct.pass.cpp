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

template <class Alloc>
void testDoesNotUse() {
    using DoesNotUseType = DoesNotUsesAllocator<Alloc, /*Args = */1>;
    int arg = 1;
    const int& carg = arg;

    testConstructBasic<DoesNotUseType>(arg, UA_None);
    testConstructBasic<DoesNotUseType>(carg, UA_None);
    testConstructBasic<DoesNotUseType>(std::move(arg), UA_None);
    testConstructBasic<DoesNotUseType>(std::move(carg), UA_None);
}

template <class Alloc>
void testUsesLeading() {
    using UsesLeadingType = UsesAllocatorV1<Alloc, /*Args = */1>;
    int arg = 1;
    const int& carg = arg;

    testConstructBasic<UsesLeadingType>(arg, UA_AllocArg);
    testConstructBasic<UsesLeadingType>(carg, UA_AllocArg);
    testConstructBasic<UsesLeadingType>(std::move(arg), UA_AllocArg);
    testConstructBasic<UsesLeadingType>(std::move(carg), UA_AllocArg);
}

template <class Alloc>
void testUsesTrailing() {
    using UsesTrailingType = UsesAllocatorV2<Alloc, /*Args = */1>;
    int arg1 = 1;
    const int& carg = arg;

    testConstructBasic<UsesTrailingType>(arg, UA_AllocLast);
    testConstructBasic<UsesTrailingType>(carg, UA_AllocLast);
    testConstructBasic<UsesTrailingType>(std::move(arg), UA_AllocLast);
    testConstructBasic<UsesTrailingType>(std::move(carg), UA_AllocLast);
}

template <class Alloc>
void testUsesBoth() {
    using UsesBoth = UsesAllocatorV3<Alloc, /*Args = */1>;
    int arg = 1;
    const int& carg = arg;

    testConstructBasic<UsesBoth>(arg, UA_AllocArg);
    testConstructBasic<UsesBoth>(carg, UA_AllocArg);
    testConstructBasic<UsesBoth>(std::move(arg), UA_AllocArg);
    testConstructBasic<UsesBoth>(std::move(carg), UA_AllocArg);
}

int main(int, char**)
{
    {
        std::pmr::polymorphic_allocator<int> alloc;
        ASSERT_SAME_TYPE(alloc.construct(nullptr, 1), void);
        ASSERT_NOT_NOEXCEPT(alloc.construct(nullptr, 1));
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
