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

// template <class T1, class T2>
// void polymorphic_allocator<T>::construct(std::pair<T1, T2>* p)

#include <memory_resource>
#include <type_traits>
#include <cassert>

#include "test_macros.h"
#include "uses_alloc_types.h"

template <class Pair>
void testConstruct(UsesAllocatorType expected1, UsesAllocatorType expected2)
{
    std::pmr::polymorphic_allocator<int> int_alloc;
    std::pmr::polymorphic_allocator<Pair> alloc = int_alloc;

    Pair* p = alloc.allocate(1);

    int_alloc.construct(p);

    checkConstruct(p->first, expected1);
    checkConstruct(p->second, expected2);

    p->~Pair();
    alloc.deallocate(p, 1);
}

template <class Alloc>
void testDoesNotUseAndDoesNotUse() {
    using DoesNotUseType = NotUsesAllocator<Alloc, /*Args = */0>;
    using PairType = std::pair<DoesNotUseType, DoesNotUseType>;
    testConstruct<PairType>(UA_None, UA_None);
}

template <class Alloc>
void testDoesNotUseAndUsesLeading() {
    using DoesNotUseType = NotUsesAllocator<Alloc, /*Args = */0>;
    using UsesLeadingType = UsesAllocatorV1<Alloc, /*Args = */0>;
    using PairType1 = std::pair<DoesNotUseType, UsesLeadingType>;
    using PairType2 = std::pair<UsesLeadingType, DoesNotUseType>;
    testConstruct<PairType1>(UA_None, UA_AllocArg);
    testConstruct<PairType2>(UA_AllocArg, UA_None);
}

template <class Alloc>
void testDoesNotUseAndUsesTrailing() {
    using DoesNotUseType = NotUsesAllocator<Alloc, /*Args = */0>;
    using UsesTrailingType = UsesAllocatorV2<Alloc, /*Args = */0>;
    using PairType1 = std::pair<DoesNotUseType, UsesTrailingType>;
    using PairType2 = std::pair<UsesTrailingType, DoesNotUseType>;
    testConstruct<PairType1>(UA_None, UA_AllocLast);
    testConstruct<PairType2>(UA_AllocLast, UA_None);
}

template <class Alloc>
void testDoesNotUseAndUsesBoth() {
    using DoesNotUseType = NotUsesAllocator<Alloc, /*Args = */0>;
    using UsesBothType = UsesAllocatorV3<Alloc, /*Args = */0>;
    using PairType1 = std::pair<DoesNotUseType, UsesBothType>;
    using PairType2 = std::pair<UsesBothType, DoesNotUseType>;
    testConstruct<PairType1>(UA_None, UA_AllocArg);
    testConstruct<PairType2>(UA_AllocArg, UA_None);
}

template <class Alloc>
void testUsesLeadingAndUsesLeading() {
    using UsesLeadingType = UsesAllocatorV1<Alloc, /*Args = */0>;
    using PairType = std::pair<UsesLeadingType, UsesLeadingType>;
    testConstruct<PairType>(UA_AllocArg, UA_AllocArg);
}

template <class Alloc>
void testUsesLeadingAndUsesTrailing() {
    using UsesLeadingType = UsesAllocatorV1<Alloc, /*Args = */0>;
    using UsesTrailingType = UsesAllocatorV2<Alloc, /*Args = */0>;
    using PairType1 = std::pair<UsesLeadingType, UsesTrailingType>;
    using PairType2 = std::pair<UsesTrailingType, UsesLeadingType>;
    testConstruct<PairType1>(UA_AllocArg, UA_AllocLast);
    testConstruct<PairType2>(UA_AllocLast, UA_AllocArg);
}

template <class Alloc>
void testUsesLeadingAndUsesBoth() {
    using UsesLeadingType = UsesAllocatorV1<Alloc, /*Args = */0>;
    using UsesBothType = UsesAllocatorV3<Alloc, /*Args = */0>;
    using PairType1 = std::pair<UsesLeadingType, UsesBothType>;
    using PairType2 = std::pair<UsesBothType, UsesLeadingType>;
    testConstruct<PairType1>(UA_AllocArg, UA_AllocArg);
    testConstruct<PairType2>(UA_AllocArg, UA_AllocArg);
}

template <class Alloc>
void testUsesTrailingAndUsesTrailing() {
    using UsesTrailingType = UsesAllocatorV2<Alloc, /*Args = */0>;
    using PairType = std::pair<UsesTrailingType, UsesTrailingType>;
    testConstruct<PairType>(UA_AllocLast, UA_AllocArg);
    testConstruct<PairType>(UA_AllocArg, UA_AllocLast);
}

template <class Alloc>
void testUsesTrailingAndUsesBoth() {
    using UsesTrailingType = UsesAllocatorV2<Alloc, /*Args = */0>;
    using UsesBothType = UsesAllocatorV3<Alloc, /*Args = */0>;
    using PairType1 = std::pair<UsesTrailingType, UsesBothType>;
    using PairType2 = std::pair<UsesBothType, UsesTrailingType>;
    testConstruct<PairType1>(UA_AllocLast, UA_AllocArg);
    testConstruct<PairType1>(UA_AllocArg, UA_AllocLast);
}

int main(int, char**)
{
    {
        std::pmr::polymorphic_allocator<int> alloc;
        std::pair<int, int>* pair_ptr = nullptr;
        ASSERT_SAME_TYPE(decltype(alloc.construct(pair_ptr)), void);
        ASSERT_NOT_NOEXCEPT(alloc.construct(pair_ptr));
    }
    {
        testDoesNotUseAndDoesNotUse<std::pmr::memory_resource*>();
        testDoesNotUseAndDoesNotUse<std::pmr::polymorphic_allocator<int>>();
    }
    {
        testDoesNotUseAndUsesLeading<std::pmr::memory_resource*>();
        testDoesNotUseAndUsesLeading<std::pmr::polymorphic_allocator<int>>();
    }
    {
        testDoesNotUseAndUsesTrailing<std::pmr::memory_resource*>();
        testDoesNotUseAndUsesTrailing<std::pmr::polymorphic_allocator<int>>();
    }
    {
        testDoesNotUseAndUsesBoth<std::pmr::memory_resource*>();
        testDoesNotUseAndUsesBoth<std::pmr::polymorphic_allocator<int>>();
    }
    {
        testUsesLeadingAndUsesLeading<std::pmr::memory_resource*>();
        testUsesLeadingAndUsesLeading<std::pmr::polymorphic_allocator<int>>();
    }
    {
        testUsesLeadingAndUsesTrailing<std::pmr::memory_resource*>();
        testUsesLeadingAndUsesTrailing<std::pmr::polymorphic_allocator<int>>();
    }
    {
        testUsesLeadingAndUsesBoth<std::pmr::memory_resource*>();
        testUsesLeadingAndUsesBoth<std::pmr::polymorphic_allocator<int>>();
    }
    {
        testUsesTrailingAndUsesTrailing<std::pmr::memory_resource*>();
        testUsesTrailingAndUsesTrailing<std::pmr::polymorphic_allocator<int>>();
    }
    {
        testUsesTrailingAndUsesBoth<std::pmr::memory_resource*>();
        testUsesTrailingAndUsesBoth<std::pmr::polymorphic_allocator<int>>();
    }

    return 0;
}
